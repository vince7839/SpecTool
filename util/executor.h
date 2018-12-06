#ifndef EXECUTOR_H
#define EXECUTOR_H

/*
 * @Author liaowenxing
 * 这个类是一个执行器类，所有的Linux指令需要通过此类来执行，分离出来便于跨平台
 */

#include<QObject>
class Executor
{
public:
    Executor();
    static void execInTerminal(QString cmd);
    static void execInBack(QString cmd,QStringList arg);
    static QString waitFinish(QString cmd,QStringList arg = QStringList());
};

#endif // EXECUTOR_H
