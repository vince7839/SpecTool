#include "specbuilder.h"
#include<test/spectest.h>
#include<test/proptest.h>
#include<test/packagetest.h>
#include<util/specutil.h>
#include<util/spectestthread.h>
#include<widget/resultwidget.h>
#include<test/defaulttest.h>
#include<QDebug>
#include<test/cmdtest.h>
#include<test/equaltest.h>
#include<test/intenttest.h>
#include<test/featuretest.h>
#include<test/sdktest.h>
SpecBuilder::SpecBuilder()
{
    util =  SpecUtil::getInstance(device);
}

/*
 * 初始化手机序列号
 */
SpecBuilder *SpecBuilder::buildDevice(QString device)
{
    this->device = device;
    return this;
}

/*
 * 是否Express+项目
 */
SpecBuilder *SpecBuilder::buildExpress(bool isExpress)
{
    this->isExpress = isExpress;
    return this;
}

/*
 * 测试类型，规范测试 or SDK测试
 */
SpecBuilder *SpecBuilder::buildTestType(QString testType)
{
    this->testType = testType;
    return this;
}

/*
 * 使用构建的条件生成测试list
 */
QList<SpecTest *> SpecBuilder::create()
{
    list.clear();
    if(testType == "SDK_TEST"){
        int expect = util->isGoVersion() ? 27 : 26;
        foreach (QString package, util->getPackages()) {
            list.append(new SdkTest(device,package,expect));
        }
        return list;
    }

    //添加公共的测试条例
    addCommonTests();

    if(util->isGoVersion()){
        //只为Go项目添加的测试条例
        addGoTests();
    }else{
        //只为非Go项目添加的测试条例
        addNonGoTests();
    }
    if(isExpress){
        //为Express+项目添加的测试条例
        addExpressTests();
    }
    return list;
}

void SpecBuilder::addCommonTests()
{
    list.append(new DefaultTest(device,DefaultTest::SPEC_IS_GMS_GO));
    //    list.append(new FeatureTest(device,"com.google.android.feature.EEA_DEVICE",QString::fromUtf8("com.google.android.feature.EEA_DEVICE(欧盟)")));
    //   list.append(new FeatureTest(device,"com.google.android.paid.search",QString::fromUtf8("com.google.android.paid.search(欧盟)")));
    //   list.append(new FeatureTest(device,"com.google.android.paid.chrome",QString::fromUtf8("com.google.android.paid.chrome(欧盟)")));
    list.append(new PropTest(device,PropTest::PROP_SECURITY,QString::fromUtf8("安全Patch")));
    list.append(new PropTest(device,PropTest::PROP_BOARD));
    list.append(new PropTest(device,PropTest::PROP_BRAND));
    list.append(new PropTest(device,PropTest::PROP_DEVICE));
    list.append(new PropTest(device,PropTest::PROP_MANUFACTURER));
    list.append(new PropTest(device,PropTest::PROP_MODEL));
    list.append(new PropTest(device,PropTest::PROP_NAME));
    list.append(new PropTest(device,PropTest::PROP_GMS_VERSION,QString::fromUtf8("GMS包版本")));
    list.append(new PropTest(device,PropTest::PROP_FINGERPRINT_1));
    list.append(new PropTest(device,PropTest::PROP_FINGERPRINT_2));
    list.append(new PropTest(device,PropTest::PROP_FINGERPRINT_3));
    list.append(new PropTest(device,PropTest::PROP_CLIENTID));
    //   list.append(new PropTest(device,PropTest::PROP_CLIENTID_AM));
    //   list.append(new PropTest(device,PropTest::PROP_CLIENTID_YT));
    list.append(new PropTest(device,PropTest::PROP_RCSA,"ACSA",util->hasPackage(PackageTest::GOOGLE_MESSAGE) ? "true" : "false"));
    list.append(new PropTest(device,PropTest::PROP_LOW_RAM,QString::fromUtf8("是否低内存（LOW RAM）"),util->isGoVersion() ? "true":"false"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_USER,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_RELEASE,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_NAME,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_DEVICE,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_SAME,"Yes"));
    list.append(new EqualTest(QString::fromUtf8("model是否一致"),util->getProp(PropTest::PROP_MODEL),util->getProp(PropTest::PROP_VENDOR_MODEL),true));
    list.append(new EqualTest(QString::fromUtf8("device是否一致"),util->getProp(PropTest::PROP_DEVICE),util->getProp(PropTest::PROP_VENDOR_DEVICE),true));
    list.append(new EqualTest(QString::fromUtf8("name是否一致"),util->getProp(PropTest::PROP_NAME),util->getProp(PropTest::PROP_VENDOR_NAME),true));
    list.append(new EqualTest(QString::fromUtf8("manufacturer是否一致"),util->getProp(PropTest::PROP_MANUFACTURER),util->getProp(PropTest::PROP_VENDOR_MANUFACTURER),true));
    list.append(new EqualTest(QString::fromUtf8("brand是否一致"),util->getProp(PropTest::PROP_BRAND),util->getProp(PropTest::PROP_VENDOR_BRAND),true));
    list.append(new DefaultTest(device,DefaultTest::SPEC_API_LEAVEL));
    list.append(new DefaultTest(device,DefaultTest::SPEC_DATA_SIZE));
    list.append(new DefaultTest(device,DefaultTest::SPEC_SYSTEM_AVAILABLE));
    list.append(new DefaultTest(device,DefaultTest::SPEC_APP_LINK));
    list.append(new DefaultTest(device,DefaultTest::SPEC_ILLEGAL_FONTS));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_INCREMENTAL));
    list.append(new DefaultTest(device,DefaultTest::SPEC_DEFAULT_ASSISTANT));
    list.append(new DefaultTest(device,DefaultTest::SPEC_PATCH_VALID,"Yes"));
    list.append(new PackageTest(device,"com.google.android.ims",true,"Carrier Service"));
    list.append(new PackageTest(device,PackageTest::GOOGLE_MESSAGE));
    list.append(new PackageTest(device,PackageTest::CHROME));
    list.append(new PackageTest(device,PackageTest::PLAY_STORE));
    list.append(new PackageTest(device,PackageTest::NAV_GO,true));
}

void SpecBuilder::addGoTests()
{
    list.append(new PackageTest(device,PackageTest::GOOGLE_GO));
    list.append(new PackageTest(device,PackageTest::YOUTUBE_GO));
    list.append(new PackageTest(device,PackageTest::GMAIL_GO));
    list.append(new PackageTest(device,PackageTest::MAPS_GO));
    list.append(new PackageTest(device,PackageTest::ASSITANT_GO));
}

void SpecBuilder::addNonGoTests()
{
    list.append(new PackageTest(device,PackageTest::GOOGLE));
    list.append(new PackageTest(device,PackageTest::YOUTUBE));
    list.append(new PackageTest(device,PackageTest::GMAIL));
    list.append(new PackageTest(device,PackageTest::MAPS));
    list.append(new PackageTest(device,PackageTest::DRIVE));
    list.append(new PackageTest(device,PackageTest::PLAY_MUSIC));
    list.append(new PackageTest(device,PackageTest::PLAY_MOVIES));
    list.append(new PackageTest(device,PackageTest::PHOTOS));
    list.append(new PackageTest(device,PackageTest::DUO));
}

void SpecBuilder::addExpressTests()
{
    if(util->isGoVersion()){
        list.append(new IntentTest(device,IntentTest::Keyboard_1));
    }else{
        list.append(new IntentTest(device,IntentTest::Gallery_3));
        list.append(new IntentTest(device,IntentTest::Gallery_4));
        list.append(new IntentTest(device,IntentTest::Calendar));
    }
    list.append(new IntentTest(device,IntentTest::Keyboard_2));
    list.append(new IntentTest(device,IntentTest::Email_1));
    list.append(new IntentTest(device,IntentTest::Email_2));
    list.append(new IntentTest(device,IntentTest::Email_3));
    list.append(new IntentTest(device,IntentTest::Email_4));
    list.append(new IntentTest(device,IntentTest::Email_5));
    list.append(new IntentTest(device,IntentTest::Browser_1));
    list.append(new IntentTest(device,IntentTest::Browser_2));
    list.append(new IntentTest(device,IntentTest::Messaging_1));
    list.append(new IntentTest(device,IntentTest::Messaging_1));
    list.append(new IntentTest(device,IntentTest::Search));
    list.append(new IntentTest(device,IntentTest::Voice_Assistant));
    list.append(new DefaultTest(device,DefaultTest::SPEC_RAM_1G,util->isGoVersion() ? "Yes" : "No"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_IS_EXPRESS));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_LIMIT,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_MANUFACTURER_LIMIT,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_MTKLOG_VERSION,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FOTA_VERSION,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_LOCATION_MODE));
    list.append(new DefaultTest(device,DefaultTest::SPEC_BLUETOOTH_OFF));
    list.append(new DefaultTest(device,DefaultTest::SPEC_ROTATION_OFF));
    list.append(new PackageTest(device,"com.android.wallpaper.livepicker",false));
    list.append(new PackageTest(device,"com.android.facelock",false));
}
