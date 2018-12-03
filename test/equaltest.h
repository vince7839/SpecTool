#ifndef EQUALTEST_H
#define EQUALTEST_H

/*
 * @Author liaowenxing
 * 此类用于判断给定的两个字符串是否相等
 */

#include<QObject>
#include<test/spectest.h>

class EqualTest : public SpecTest
{
public:
    EqualTest(QString name,QString a,QString b,bool expect=true);
    QString getName();
    QString getResult();
    QString getExpect();
    Status getStatus();
    void run();
private:
    bool expect;
    QString name;
    QString result;
    Status status;
};

#endif // EQUALTEST_H
