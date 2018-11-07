#ifndef EQUALTEST_H
#define EQUALTEST_H

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
