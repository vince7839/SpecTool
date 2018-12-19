#include "defaulttest.h"
#include"util/specutil.h"
#include<test/proptest.h>
#include<util/executor.h>
#include<QDebug>
#include<QDir>
#include<test/packagetest.h>
#include<test/intenttest.h>
#include<QThread>
DefaultTest::DefaultTest(QString device,SpecType type, QString expect)
{
    this->device = device;
    this->util = SpecUtil::getInstance(device);
    this->type = type;
    this->expect = expect;
}

void DefaultTest::run()
{
    switch(type){
    case SPEC_IS_RU:
    {
        QString FEATURE_RU = "com.google.android.feature.RU";
        name = QString::fromUtf8("是否俄罗斯市场");
        result = util->hasFeature(FEATURE_RU) ? "Yes" : "No" ;
    }
        break;
    case SPEC_IS_GMS_GO:
        name = QString::fromUtf8("GMS包是否为Go版本");
        result =util->isGoVersion() ? "Yes" : "No";
        status = PASS;
        break;
    case SPEC_FINGERPRINT_USER:
        name = QString::fromUtf8("fingerprint是否为user");
        result = util->fingerprintAttr(4);
        status = result.contains("user") && !result.contains("userdebug")? PASS : FAIL;
        break;
    case SPEC_FINGERPRINT_RELEASE:
        name = QString::fromUtf8("fingerprint是否为release-key");
        result = util->fingerprintAttr(5);
        status = result == "release-keys" ? PASS : FAIL;
        break;
    case SPEC_FINGERPRINT_NAME:{
        name = QString::fromUtf8("fingerprint和ro值的name是否一致");
        QString attr = util->fingerprintAttr(1);
        QString prop = util->getProp(PropTest::PROP_NAME);
        status = attr == prop  ? PASS : FAIL;
        result = QString("%1%2%3").arg(attr).arg(QString::fromUtf8(status == PASS ? "等于" : "不等于")).arg(prop);

    }
        break;
    case SPEC_FINGERPRINT_DEVICE:{
        name = QString::fromUtf8("fingerprint和ro值的device是否一致");
        QString field = util->fingerprintAttr(2) .split(":").first();
        QString prop = util->getProp(PropTest::PROP_DEVICE);
        status = field == prop ? PASS : FAIL;
        result = QString("%1%2%3").arg(field).arg(QString::fromUtf8(status == PASS ? "等于" : "不等于")).arg(prop);
        break;
    }
    case SPEC_FINGERPRINT_SAME:{
        name = QString::fromUtf8("3个fingerprint是否一致");
        bool isSame = util->getProp(PropTest::PROP_FINGERPRINT_1) == util->getProp(PropTest::PROP_FINGERPRINT_2)
                && util->getProp(PropTest::PROP_FINGERPRINT_2) == util->getProp(PropTest::PROP_FINGERPRINT_3) ;
        result = isSame ? "Yes" : "No";
        status = isSame ? PASS : FAIL;
        break;
    }
    case SPEC_FINGERPRINT_INCREMENTAL:{
        name = QString::fromUtf8("fingerprint和ro值的incremental是否一致");
        QString attr = util->fingerprintAttr(4).simplified().split(":").first();
        QString prop = util->getProp(PropTest::PROP_INCREMENTAL);
        status = attr == prop ? PASS: FAIL;
        result = QString("%1%2%3").arg(attr).arg(QString::fromUtf8(status == PASS ? "等于" : "不等于")).arg(prop);
        expect = "Yes";
    }
        break;
    case SPEC_IS_EXPRESS:
        name = QString::fromUtf8("GMSEXPRESS_PLUS_BUILD是否写入");
        expect = "Yes";
        result = util->isExpress() ? "Yes" : "No";
        status = expect == result ? PASS : FAIL;
        break;
    case SPEC_OEM_UNLOCK:{
        name = QString::fromUtf8("OEM Unlocking默认关闭");
        Executor::waitFinish(QString("adb -s %1 reboot bootloader").arg(device));
        QString output = Executor::waitFinish("fastboot flashing get_unlock_ability");
        result = output.contains("unlock_ability = 0") ? "Yes" : "No";
        status = result == "Yes" ? PASS: FAIL;
        expect = "Yes";
        Executor::waitFinish("fastboot reboot");
    }
        break;
    case SPEC_RAM_1G:
        name = QString::fromUtf8("RAM是否小于1G");
        result =  util->ramLimit() ? "Yes" : "No";
        status = result == expect ? PASS : FAIL;
        break;
    case SPEC_PATCH_VALID:
    {
        int DAY_LIMIT = util->isExpress() ? 20 : 20;
        name = QString::fromUtf8("安全Patch距离截止日期是否%1天之内").arg(DAY_LIMIT);
        int remainingDay = util->patchRemainingDay();
        if(remainingDay < 0){
            status = FAIL;
            result = QString::fromUtf8("已超出截止日期%1天").arg(-remainingDay);
        }else{
            status = remainingDay < DAY_LIMIT ? WARNING : PASS;
            result = QString::fromUtf8("距离截止日期还有%1天").arg(remainingDay);
        }
    }
        break;
    case SPEC_GMS_VALID:
    {
        const int DAY_LIMIT = 20;
        name = QString::fromUtf8("GMS包版本距离截止日期是否%1天之内").arg(DAY_LIMIT);
        QDate now = QDate::currentDate();
        QDate dueDate = util->gmsDeadline();
        if(dueDate.isNull()){
            result = QString::fromUtf8("未从服务器获取到截止日期");
            status = WARNING;
        }else{
            int days = now.daysTo(dueDate);
            if(days < 0){
                status = FAIL;
                result = QString::fromUtf8("已超过截止日期%1天").arg(-days);
            }else{
                status = days < DAY_LIMIT ? WARNING : PASS;
                result = QString::fromUtf8("距离截止日期还有%1天").arg(days);
            }
        }
    }
        break;
    case SPEC_FINGERPRINT_LIMIT:{
        name = QString::fromUtf8("fingerprint命名规范");
        expect = QString::fromUtf8("不含andorid,google,alps等");
        result = util->getProp(PropTest::PROP_FINGERPRINT_1);
        bool exist = result.contains("google",Qt::CaseInsensitive) || result.contains("android",Qt::CaseInsensitive)
                || result.contains("alps",Qt::CaseInsensitive);
        status = exist ? FAIL: PASS;
    }
        break;
    case SPEC_MANUFACTURER_LIMIT:{
        name = QString::fromUtf8("manufacturer命名规范");
        expect = QString::fromUtf8("不含andorid,google,alps等");
        result  = util->getProp(PropTest::PROP_MANUFACTURER);
        bool exist = result.contains("google",Qt::CaseInsensitive) || result.contains("android",Qt::CaseInsensitive)
                || result.contains("alps",Qt::CaseInsensitive);
        status = exist ? FAIL : PASS;
    }
        break;
    case SPEC_MTKLOG_VERSION:{
        name = QString::fromUtf8("mtklog版本>=4.20");
        result = Executor::waitFinish("adb shell dumpsys package com.mediatek.mtklogger|grep versionName").trimmed().split("=").last();
        status = result >= "4.2" ? PASS : FAIL;
    }
        break;
    case SPEC_FOTA_VERSION:{
        name = QString::fromUtf8("广生FOTA版本>=5.22");
        if(util->hasPackage("com.adups.fota")){
            result = Executor::waitFinish("adb shell dumpsys package com.adups.fota|grep versionName").trimmed().split("=").last();
            status = result >= "5.22" ? PASS : FAIL;
        }else{
            result = QString::fromUtf8("未预置");
            status = PASS;
        }
    }
        break;
    case SPEC_YOUTUBE_GO:
    {
        name = QString::fromUtf8("Youtobe Go版本>=1.18.57");
        result = Executor::waitFinish("adb shell dumpsys package com.google.android.apps.youtube.mango|grep versionName").trimmed().split("=").last();
        status = result >= "1.18.57" ? PASS : FAIL;
    }
        break;
    case SPEC_DATA_SIZE:
    {
        name = QString::fromUtf8("data分区大小");
        int kb = util->dataSize();
        float limitGb = 0;
        if(kb > 1024*1024*8) {
            limitGb = 11;
        }else if(kb > 1024*1024*4) {
            limitGb = 5.1;
        }else{
            limitGb = 1.5;
        }
        expect = QString::fromUtf8("> %1 GB").arg(limitGb);
        status = kb > limitGb*1024*1024 ? PASS : FAIL;
        result = QString("%1 GB").arg(((float)kb)/(1024*1024));
    }
        break;
    case SPEC_LOCATION_MODE:
        name = QString::fromUtf8("location默认高精度");
        expect = "Yes";
        result = util->getSetting("secure","location_providers_allowed").simplified().split(",").size() == 2 ? "Yes" : "No";
        status = result == "Yes" ? PASS : FAIL;
        break;
    case SPEC_BLUETOOTH_OFF:
        name = QString::fromUtf8("蓝牙默认关闭");
        expect = "Yes";
        status = util->getSetting("global","bluetooth_on") == "0" ? PASS : FAIL;
        result = status == PASS ? "Yes" : "No";
        break;
    case SPEC_ROTATION_OFF:
        name = QString::fromUtf8("自动旋转默认关闭");
        expect = "Yes";
        status = util->getSetting("system","accelerometer_rotation") == "0" ? PASS : FAIL;
        result = status == PASS ? "Yes" : "No";
        break;
    case SPEC_COMPANY_LIMIT:{
        name = QString::fromUtf8("ro值是否可以含有Mobiwire等字样");
        QString ro = util->companyLimit();
        result = ro.isEmpty() ? "No" : QString::fromUtf8("%1->%2").arg(ro).arg(util->getProp(ro));
        if(expect == "No" && !ro.isEmpty()){
            status = FAIL;
        }else{
            status = PASS;
        }
    }
        break;
    case SPEC_SYSTEM_AVAILABLE:{
        int dataKb = util->dataSize();
        int systemAvailableKb = util->systemAvailable();
        int limitMb = 0;
        if(dataKb >= 1024*1024*4){
            limitMb = 220;
        }else{
            limitMb = 130;
        }
        name = QString::fromUtf8("System区可用空间");
        status = systemAvailableKb > limitMb*1024 ? PASS : FAIL;
        expect = QString("> %1 MB").arg(limitMb);
        result = QString("%1 MB").arg((float)systemAvailableKb/1024);
    }
        break;
    case SPEC_API_LEAVEL:
        expect = util->expectApiLevel();
        result = util->getProp(PropTest::PROP_API_LEVEL);
        status =  result == expect ? PASS : WARNING;
        name = QString::fromUtf8("First API Level（量产软件此项为空）");
        break;
    case SPEC_APP_LINK:{
        name = QString::fromUtf8("google.xml中APP Link检测");
        QString filename = "google.xml";
        Executor::waitFinish(QString("adb -s %1 pull /etc/sysconfig/google.xml %2").arg(device).arg(filename));
        QFile file(filename);
        if(file.exists() && file.open(QIODevice::ReadOnly)){
            QString content = file.readAll();
            status = FAIL;
            if(util->hasPackage(PackageTest::YOUTUBE)){
                status = content.contains(PackageTest::YOUTUBE) ? PASS : FAIL;
                result = QString::fromUtf8(status == PASS ? "Youtube已配置" : "Youtube未配置");
                expect = QString::fromUtf8("配置Youtube");
            }else if(util->hasPackage(PackageTest::YOUTUBE_GO)){
                status = content.contains(PackageTest::YOUTUBE_GO) ? PASS : FAIL;
                result = QString::fromUtf8(status == PASS ? "Youtube Go已配置" : "Youtube Go未配置");
                expect = QString::fromUtf8("配置Youtube Go");
            }else{
                result = QString::fromUtf8("项目未预置youtube应用");
                status = WARNING;
            }
            file.close();
            file.remove();
        }else{
            result = QString::fromUtf8("未读取到google.xml");
        }
    }
        break;
    case SPEC_ILLEGAL_FONTS:
    {
        name = QString::fromUtf8("第三方字体检测");
        expect = QString::fromUtf8("未预置");
        status = PASS;
        foreach(QString package,util->getPackages()){
            if(package.contains("delux",Qt::CaseInsensitive)||package.contains("font",Qt::CaseInsensitive)){
                status = FAIL;
                result.append(package).append("\n");
            }
        }
        if(status == PASS){
            result = QString::fromUtf8("未预置");
        }
    }
        break;
    case SPEC_DEFAULT_ASSISTANT:
    {
        name = QString::fromUtf8("Google Assistant是否默认语音助手");
        expect = "Yes";
        QFile file(":/resource/intent_test_apk");
        QString copyName = "IntentTest.apk";
        file.copy(copyName);
        if(!QFile::exists(copyName)){
            status = FAIL;
            result = QString::fromUtf8("无法安装测试APK");
            break;
        }else{
            Executor::waitFinish(QString("adb -s %1 install %2").arg(device).arg(copyName));
        }
        QString package = util->isGoVersion() ? PackageTest::ASSITANT_GO : PackageTest::QUICK_SEARCH_BOX;
        Executor::waitFinish(QString("adb -s %1 shell input keyevent --longpress KEYCODE_HOME").arg(device));
        QThread::sleep(2);
        QString output = Executor::waitFinish(QString("adb -s %1 shell dumpsys activity").arg(device));
        QRegExp re("Recent #0: TaskRecord\\{.*\\}");
        re.setMinimal(true);
        if(output.indexOf(re) != -1){
            status = re.cap(0).contains(package) ? PASS : FAIL;
            qDebug()<<re.cap(0);
        }else{
            status = FAIL;
        }
        result = status == PASS ? "Yes" : "No";
        Executor::waitFinish(QString("adb -s %1 shell am force-stop %2").arg(device).arg(package));
        Executor::waitFinish(QString("adb -s %1 uninstall %2").arg(device).arg("com.sagereal.intenttest"));
        QFile::remove(copyName);
    }
        break;
        // 2018/11/28 zhaocongcong 添加EEA规范检查
    case SPEC_MAPVIEW_V1:
    {
        name = QString::fromUtf8("MapView V1 Library检测");
        expect = QString::fromUtf8("存在");
        QString content = Executor::waitFinish(QString("adb -s %1 shell \"find /system/framework/ -name com.google.android.maps.jar\"").arg(device));
        status = content.isEmpty() ? FAIL : PASS;
        result = QString::fromUtf8(status == PASS ? "存在" : "不存在");
    }
        break;
    case SPEC_GOOGLE_MEDIA_EFFECTS:
    {
        name = QString::fromUtf8("Google Media Effects Library检测");
        expect = QString::fromUtf8("存在");
        QString content = Executor::waitFinish(QString("adb -s %1 shell \"find /system/framework/ -name com.google.android.media.effects.jar\"").arg(device));
        status = content.isEmpty() ? FAIL : PASS;
        result = QString::fromUtf8(status == PASS ? "存在" : "不存在");
    }
        break;
    case SPEC_OPENMOBILEAPI:
    {
        name = QString::fromUtf8("Open Mobile API服务是否存在");
        QString content = Executor::waitFinish(QString("adb -s %1 shell service list").arg(device));
        status = content.contains("simalliance.openmobileapi.service") ? FAIL : PASS;
        result = status == PASS ? "No" : "Yes";
        expect = "No";
    }
        break;
    case SPEC_IS_EEA:
    {
        const QString PAID_SEARCH = "com.google.android.paid.search";
        const QString PAID_CHROME = "com.google.android.paid.chrome";
        name = QString::fromUtf8("是否为欧盟市场");
        bool eea = util->isEEA();
        bool paidSearch = util->hasFeature(PAID_SEARCH);
        bool paidChrome = util->hasFeature(PAID_CHROME);
        if(eea){
            status = paidChrome && paidSearch ? PASS : FAIL;
            result =  QString::fromUtf8("%1满足Type 4c要求").arg(paidChrome && paidSearch ? "" : "不");
        }else{
            status = PASS;
            result = "No";
        }
    }
        break;
    }
}

QString DefaultTest::getName()
{
    return name;
}

QString DefaultTest::getExpect()
{
    return expect;
}

QString DefaultTest::getResult()
{
    return result;
}

SpecTest::Status DefaultTest::getStatus()
{
    return status;
}
