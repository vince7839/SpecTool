#include "defaulttest.h"
#include"util/specutil.h"
#include<test/proptest.h>
#include<util/executor.h>
#include<QDebug>
#include<QDir>
#include<test/packagetest.h>
const QString DefaultTest::FEATURE_RU = "com.google.android.feature.RU";

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
        name = QString::fromUtf8("是否俄罗斯市场");
        result = util->hasFeature(FEATURE_RU) ? "Yes" : "No" ;
        break;
    case SPEC_IS_GMS_GO:
        name = QString::fromUtf8("GMS包是否为Go版本");
        result =util->isGoVersion() ? "Yes" : "No";
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
        name = QString::fromUtf8("安全Patch是否1个月之内");
        status = util->patchValid() ? PASS : FAIL;
        result = util->getProp(PropTest::PROP_SECURITY);
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
        result = Executor::waitFinish("adb shell dumpsys package com.adups.fota|grep versionName").trimmed().split("=").last();
        status = result >= "5.22" ? PASS : FAIL;
    }
        break;
    case SPEC_DATA_SIZE:
    {
        name = QString::fromUtf8("data分区大小");
        int kb = util->dataSize();
        float limitGb = 0;
        if(kb > 1024*1024*4) {
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
        name = QString::fromUtf8("First API Level（MP软件可不填）");
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
