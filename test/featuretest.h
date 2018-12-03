#ifndef FEATURETEST_H
#define FEATURETEST_H

/*
 * @Author liaowenxing
 * 这个类判断手机是否含有某个Feature
 */
#include<test/spectest.h>

class FeatureTest : public SpecTest
{
public:
    FeatureTest(QString device,QString feature,QString name,QString expect = "");
    QString getName();
    QString getResult();
    QString getExpect();
    Status getStatus();
    void run();
private:
    QString device;
    QString feature;
    QString expect;
    QString result;
    Status status;
    QString name;
};

#endif // FEATURETEST_H
