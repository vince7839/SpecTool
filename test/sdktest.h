#ifndef SDKTEST_H
#define SDKTEST_H

/*
 * @Author liaowenxing
 * 此类判断某个apk的target SDK是否是我们的期望值
 */
#include<test/spectest.h>
class SdkTest : public SpecTest
{
public:
    SdkTest(QString device,QString package,int expect);
    QString getExpect();
    QString getName();
    QString getResult();
    Status getStatus();
    void run();
private:
    QString package;
    QString result;
    int expect;
    SpecTest::Status status;
    QString device;
};

#endif // SDKTEST_H
