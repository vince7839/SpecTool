#ifndef INTENTTEST_H
#define INTENTTEST_H

/*
 * @Author liaowenxing
 * 这个类主要用于Express+项目，发送一个广播来启动某个app
 */

#include <QObject>
#include<test/spectest.h>
#include<util/specutil.h>
class IntentTest : public SpecTest
{
public:
    enum IntentType{
        Browser_1,
        Browser_2,
        Gallery_1,
        Gallery_2,
        Gallery_3,
        Gallery_4,
        Messaging_1,
        Messaging_2,
        Email_1,
        Email_2,
        Email_3,
        Email_4,
        Email_5,
        Keyboard_1,
        Keyboard_2,
        Voice_Assistant,
        Search,
        Calendar
    };
    IntentTest(QString device,IntentType type);
    void run();
    QString getName();
    QString getResult();
    QString getExpect();
    SpecTest::Status getStatus();
    QString getCmd();
private:
    QString name;
    QString expect;
    QString result;
    Status status;
    QString device;
    IntentType type;
    SpecUtil*util;
    QString cmd;
};

#endif // INTENTTEST_H
