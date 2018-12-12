#ifndef DEFAULTTEST_H
#define DEFAULTTEST_H

/*
 * @Author liaowenxing
 * 这个类主要是一些无法使用CmdTest或者PackageTest等归类的测试
 */

#include<test/spectest.h>
#include<util/specutil.h>
class DefaultTest : public SpecTest
{
public:
    static const QString FEATURE_RU;
    static const QString EEA_DEVICE;
    static const QString PAID_SEARCH;
    static const QString PAID_CHROME;
    enum SpecType{
        SPEC_IS_GMS_GO,
        SPEC_IS_RU,
        SPEC_IS_EXPRESS,
        SPEC_FINGERPRINT_INCREMENTAL,
        SPEC_FINGERPRINT_USER,
        SPEC_FINGERPRINT_RELEASE,
        SPEC_FINGERPRINT_NAME,
        SPEC_FINGERPRINT_DEVICE,
        SPEC_FINGERPRINT_SAME,
        SPEC_FINGERPRINT_LIMIT,
        SPEC_MANUFACTURER_LIMIT,
        SPEC_OEM_UNLOCK,
        SPEC_RAM_1G,
        SPEC_PATCH_VALID,
        SPEC_MTKLOG_VERSION,
        SPEC_FOTA_VERSION,
        SPEC_DATA_SIZE,
        SPEC_LOCATION_MODE,
        SPEC_BLUETOOTH_OFF,
        SPEC_ROTATION_OFF,
        SPEC_COMPANY_LIMIT,
        SPEC_SYSTEM_AVAILABLE,
        SPEC_API_LEAVEL,
        SPEC_APP_LINK,
        SPEC_ILLEGAL_FONTS,
        SPEC_DEFAULT_ASSISTANT,
        // 2018/11/28 zhaocongcong 添加 EEA规范检查
        SPEC_MAPVIEW_V1,
        SPEC_GOOGLE_MEDIA_EFFECTS,
        SPEC_OPENMOBILEAPI,
        // 2018/12/04 zhaocongcong 添加 EEA_DEVICE,search,chrome的同时检测
        SPEC_EEADCS,
        // 2018/12/10 zhaocongcong 添加 GMS package截止日期检测
        SPEC_8_GMS_PACKAGE,
        // SPEC_7_GMS_PACKAGE,
        // 2018/12/10 zhaocongcong 添加 YOUTUBE_GO版本检测
        SPEC_YOUTUBE_GO,
        // 2018/12/11 zhaocongcong 添加 工具时间检测
        SPEC_CTS_CTSV_R10,
        SPEC_CTS_CTSV_R11,
        SPEC_GTS_R2,
        SPEC_GTS_R3,
        SPEC_8_STS_R12,
        SPEC_8_STS_R19_01,
        SPEC_VTS_GSI
//        SPEC_CTS_CTSV_R26,
//        SPEC_CTS_CTSV_R27,
//        SPEC_7_STS_R12,
    };
    DefaultTest(QString device,SpecType type,QString expect = "");
    void run();
    QString getName();
    QString getExpect();
    QString getResult();
    Status getStatus();
private:
    SpecType type;
    QString result;
    QString expect;
    QString name;
    Status status = SpecTest::UNKNOWN;
    QString device;
    SpecUtil*util;
};

#endif // DEFAULTTEST_H
