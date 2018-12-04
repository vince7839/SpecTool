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
        SPEC_OPENMOBILEAPI
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
