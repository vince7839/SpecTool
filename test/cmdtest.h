#ifndef CMDTEST_H
#define CMDTEST_H

#include<test/spectest.h>

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
