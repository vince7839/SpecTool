#include "specutil.h"
#include<util/executor.h>
#include<test/proptest.h>
#include<QDebug>
#include<QDate>
#include<util/system.h>
QString SpecUtil::device;
SpecUtil* SpecUtil::sInstance;
SpecUtil::SpecUtil(QString device)
{    
    qDebug()<<device;
    packages = Executor::waitFinish(QString("adb -s %1 shell pm list packages").arg(device)).split(System::getSeparator());
    packages.removeAll("");
    packages.replaceInStrings("package:","");
    features = Executor::waitFinish(QString("adb -s %1 shell pm list features").arg(device)).split(System::getSeparator());
    features.removeAll("");
    features.replaceInStrings("feature:","");
    QStringList list = Executor::waitFinish(QString("adb -s %1 shell getprop").arg(device)).split(System::getSeparator());
    QRegExp re("\\[(.*)\\]: \\[(.*)\\]");
    foreach(QString line,list){
        if(re.exactMatch(line)){
            props.insert(re.cap(1),re.cap(2));
        }
    }
}

SpecUtil *SpecUtil::getInstance(QString device)
{
    if(device != SpecUtil::device){
        sInstance = new SpecUtil(device);
        SpecUtil::device = device;
    }
    return sInstance;
}

QString SpecUtil::getProp(QString prop)
{
    QString value =  props.value(prop);
    qDebug()<<"[SpecUtil::getProp]"<<prop<<value;
    return value;
}

QString SpecUtil::fingerprintAttr(int index)
{
    QString fp = getProp(PropTest::PROP_FINGERPRINT_1);
    qDebug()<<"[SpecUtil::fingerprintAttr]fingerprint:"<<fp;
    QStringList list = fp.split("/");
    QString field;
    if(index < list.size()){
        field = list.at(index);
    }else{
        qDebug()<<"[SpecUtil::fingerprintAttr]invalid index";
    }
    return field;
}

QString SpecUtil::expectApiLevel()
{
    QString release = getProp(PropTest::PROP_RELEASE);
    QString level = "";
    if(release.startsWith("5.0")){
        level = "21";
    }else if(release.startsWith("5.1")){
        level = "22";
    }else if(release.startsWith("6.0")){
        level = "23";
    }else if(release.startsWith("7.0")){
        level = "24";
    }else if(release.startsWith("7.1")){
        level = "25";
    }else if(release.startsWith("8.0")){
        level = "26";
    }else if(release.startsWith("8.1")){
        level = "27";
    }
    return level;
}

bool SpecUtil::isExpress()
{
    return hasFeature("com.google.android.feature.GMSEXPRESS_PLUS_BUILD");
}

bool SpecUtil::isGoVersion()
{
    return getProp(PropTest::PROP_GMS_VERSION).contains("go");
}

bool SpecUtil::isAndroid8()
{
    return getProp(PropTest::PROP_RELEASE).contains("8.");
}

bool SpecUtil::isEEA()
{
    return hasFeature("com.google.android.feature.EEA_DEVICE");
}

bool SpecUtil::ramLimit()
{
    QString output = Executor::waitFinish(QString("adb -s %1 shell cat proc/meminfo").arg(device));
    QStringList list = output.split("\n");
    QRegExp re(".*([0-9]+).*");
    if(!list.isEmpty() && re.exactMatch(list.first())){
        float kb = re.cap(1).toFloat();
        float gb = kb/(1024*1024);
        qDebug()<<kb<<gb;
        return gb <= 1.0;
    }
    return false;
}

bool SpecUtil::hasFeature(QString feature)
{
    bool exist = features.contains(feature);
    qDebug()<<"[SpecUtil::hasFeature]"<<feature<<exist;
    return exist;
}

bool SpecUtil::hasPackage(QString package)
{  qDebug()<<"------------"<<packages.size()<<package;
    bool exist = packages.contains(package);
    qDebug()<<"[SpecUtil::hasPackage]"<<package<<exist;
    return exist;
}

int SpecUtil::patchRemainingDay()
{
    int dayCount = 0;
    QString date = getProp(PropTest::PROP_SECURITY);
    QRegExp re("([0-9]+)-([0-9]+)-([0-9]+)");
    if(re.exactMatch(date)){
        //patch起始时间
        QDate patchDate = QDate::fromString(date,"yyyy-MM-dd");
        //截止时间 = patch时间加2个月
        QDate dueDate = patchDate.addMonths(2);
        //当前时间
        QDate now = QDate::currentDate();
        dayCount = now.daysTo(dueDate);
    }
    return dayCount;
}


int SpecUtil::gmsVersionDayCount()
{
    int dayCount = 0;
    QString date = getProp(PropTest::PROP_GMS_VERSION);
    QString gmsDate;
    QString data1 = "201809";
    QString data2 = "201810";
    if(date.contains(data1)){
        gmsDate = "2018-09-24";
    }else if(date.contains(data2)){
        gmsDate = "2018-10-16";
    }else{
        gmsDate = date.mid(4,4)+"-"+date.mid(8,2)+"-"+"16";
    }
    QRegExp re("([0-9]+)-([0-9]+)-([0-9]+)");
    if(re.exactMatch(gmsDate)){
        //GMSVERSION时间
        QDate gmsVersionDate = QDate::fromString(gmsDate,"yyyy-MM-dd");
        //截止时间 = GMSVERSION时间加3个月
        QDate dueDate = gmsVersionDate.addMonths(3);
        //当前时间
        QDate now = QDate::currentDate();
        if(now < dueDate){
            dayCount = qAbs(now.daysTo(dueDate));
        }else{
            dayCount = -qAbs(now.daysTo(dueDate));
        }
    }
    return dayCount;
}

int SpecUtil::dueDayCount(QString startDate,QString dueDate)
{
    int dayCount = 0;
    QRegExp re("([0-9]+)-([0-9]+)-([0-9]+)");
    if(dueDate.isEmpty()){
        if(re.exactMatch(startDate)){
            QDate dieDate = QDate::fromString(startDate,"yyyy-MM-dd");
            //截止时间 = 开始时间加2个月
            QDate dueDate = dieDate.addMonths(2);
            //当前时间
            QDate now = QDate::currentDate();
            if(now < dueDate){
                dayCount = qAbs(now.daysTo(dueDate));
            }else{
                dayCount = -qAbs(now.daysTo(dueDate));
            }
        }
    }else{
        if(re.exactMatch(dueDate)){
             QDate dieDate = QDate::fromString(dueDate,"yyyy-MM-dd");
             //当前时间
             QDate now = QDate::currentDate();
             if(now < dieDate){
                  dayCount = qAbs(now.daysTo(dieDate));
             }else{
                  dayCount = -qAbs(now.daysTo(dieDate));
             }
        }
    }
    return dayCount;
}

QString SpecUtil::getSetting(QString type, QString key)
{
    return Executor::waitFinish(QString("adb -s %1 shell settings get %2 %3").arg(device).arg(type).arg(key)).simplified();
}

QString SpecUtil::getCurrentDevice()
{
    return device;
}

QString SpecUtil::companyLimit()
{
    QStringList keys = props.keys();
    foreach (QString k, keys) {
        if(props.value(k).contains("mobiwire",Qt::CaseInsensitive)){
            return k;
        }
    }
    return "";
}

QStringList SpecUtil::getPackages()
{
    return packages;
}

int SpecUtil::dataSize()
{
    int kb = 0;
    QString line = Executor::waitFinish(QString("adb -s %1 shell df /data").arg(device)).split(System::getSeparator()).at(1);
    QStringList fields = line.simplified().split(" ");
    if(fields.size() >= 2){
        kb = fields.at(1).toInt();
    }
    qDebug()<<"data disk size"<<kb;
    return kb;
}

int SpecUtil::systemAvailable()
{
    int kb = 0;
    QString line = Executor::waitFinish(QString("adb -s %1 shell df /system").arg(device)).split(System::getSeparator()).at(1);
    QStringList fields = line.simplified().split(" ");
    if(fields.size() >= 4){
        kb = fields.at(3).toInt();
    }
    qDebug()<<"system available size:"<<kb;
    return kb;
}
