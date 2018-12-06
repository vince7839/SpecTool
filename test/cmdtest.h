#ifndef CMDTEST_H
#define CMDTEST_H

#include<test/spectest.h>

/*
 * @Author liaowenxing
 * 这个类继承自抽象类SpecTest，主要执行一些用特殊命令获取结果的测试
 */

class CmdTest : public SpecTest
{
public:
    CmdTest(QString name,QString cmd,QString expect = "");
    QString getName();
    QString getResult();
    QString getExpect();
    Status getStatus();
    void run();
private:
    QString name;
    QString cmd;
    QString expect;
    QString result;
    Status status;
};

#endif // CMDTEST_H
