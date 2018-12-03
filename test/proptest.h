#ifndef PROPTEST_H
#define PROPTEST_H

/*
 * @Author liaowenxing
 * 这个类用于判断一个ro值是否是我们期望的内容
 */

#include<QObject>
#include<test/spectest.h>
#include<util/specutil.h>
class PropTest : public SpecTest
{

public:
    static const QString PROP_LOW_RAM;
    static const QString PROP_GMS_VERSION;
    static const QString PROP_BOARD;
    static const QString PROP_BRAND;
    static const QString PROP_DEVICE;
    static const QString PROP_MANUFACTURER;
    static const QString PROP_MODEL;
    static const QString PROP_NAME;
    static const QString PROP_FINGERPRINT_1;
    static const QString PROP_FINGERPRINT_2 ;
    static const QString PROP_FINGERPRINT_3;
    static const QString PROP_CLIENTID;
    static const QString PROP_CLIENTID_AM;
    static const QString PROP_CLIENTID_YT;
    static const QString PROP_API_LEVEL;
    static const QString PROP_SECURITY ;
    static const QString PROP_RELEASE;
    static const QString PROP_DENSITY;
    static const QString PROP_INCREMENTAL;
    static const QString PROP_VENDOR_MODEL;
    static const QString PROP_VENDOR_DEVICE;
    static const QString PROP_VENDOR_NAME;
    static const QString PROP_VENDOR_MANUFACTURER;
    static const QString PROP_VENDOR_BRAND;
    static const QString PROP_RCSA;
    PropTest(QString device,QString prop,QString name = "", QString expect = "");
    void run();
    QString getName();
    QString getExpect();
    QString getResult();
    Status getStatus();
    QString getCmd();
private:
    QString name;
    QString device;
    QString result;
    QString expect;
    QString prop;
    Status status;
    SpecUtil*util = NULL;
    QString cmd;
};

#endif // PROPTEST_H
