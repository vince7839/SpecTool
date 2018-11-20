#ifndef SPECTEST_H
#define SPECTEST_H
#include<QMetaType>
#include<QObject>

class SpecTest : public QObject{

public:
    enum Status{
        PASS,
        FAIL,
        UNKNOWN,
        WARNING
    };
    virtual void run() = 0;
    virtual QString getName() = 0;
    virtual QString getExpect() = 0;
    virtual QString getResult() = 0;
    virtual Status getStatus() = 0;
    virtual QString getCmd() {return "";}
};
Q_DECLARE_METATYPE(SpecTest*)
#endif // SPECTEST_H
