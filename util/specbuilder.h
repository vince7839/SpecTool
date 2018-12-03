#ifndef SPECBUILDER_H
#define SPECBUILDER_H
#include <QObject>
#include<test/spectest.h>
#include<util/specutil.h>
class SpecBuilder
{
public:
    static const int GMS_REQUIRED = 0;
    static const int GMS_NOT_REQUIRED = 1;
    static const int SMART_FEATURE_PHONE = 2;
    SpecBuilder();
    SpecBuilder*buildDevice(QString device);
    SpecBuilder*buildExpress(bool isExpress);
    SpecBuilder*buildTestType(QString testType);
    QList<SpecTest*> create();
    void addCommonTests();
    void addGoTests();
    void addNonGoTests();
    void addExpressTests();
private:
    QString device;
    QString testType;
    bool isExpress = false;
    QList<SpecTest*> list;
    SpecUtil*util;
};

#endif // SPECBUILDER_H
