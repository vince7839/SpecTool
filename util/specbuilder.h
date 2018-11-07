#ifndef SPECBUILDER_H
#define SPECBUILDER_H
#include <QObject>
#include<test/spectest.h>
class SpecBuilder
{
public:

    static const int GMS_REQUIRED = 0;
    static const int GMS_NOT_REQUIRED = 1;
    static const int SMART_FEATURE_PHONE = 2;
    SpecBuilder();
    SpecBuilder*buildPhoneType(int type);
    SpecBuilder*buildMpType(bool isMp);
    SpecBuilder*buildDevice(QString device);
    SpecBuilder*buildExpress(bool isExpress);
    SpecBuilder*buildTestType(QString testType);
    QList<SpecTest*> create();
private:
    QString device;
    QString testType;
    bool isExpress = false;
    bool isMp = false;
    int phoneType;
};

#endif // SPECBUILDER_H
