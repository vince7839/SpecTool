#include "sdktest.h"
#include<util/executor.h>
#include<QDebug>
SdkTest::SdkTest(QString device, QString package, int expect)
{
    this->device = device;
    this->package = package;
    this->expect = expect;
}

QString SdkTest::getExpect()
{
    return QString::number(expect);
}

QString SdkTest::getName()
{
    return package;
}

QString SdkTest::getResult()
{
    return result;
}

SpecTest::Status SdkTest::getStatus()
{
    return status;
}

void SdkTest::run()
{
    QString output = Executor::waitFinish(QString("adb -s %1 shell dumpsys package %2").arg(device).arg(package));
    QRegExp re(".*targetSdk=([0-9]{2}).*");
    if(re.exactMatch(output)){
        result = re.cap(1);
        status = result.toInt() < expect ? FAIL : PASS;
        qDebug()<<result;
    }
}
