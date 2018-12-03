#ifndef SPECTEST_H
#define SPECTEST_H

/*
 * @Author liaowenxing
 * 这个类是一个纯虚类（抽象类） 不可以被实例化 它是所有测试类的父类 要实现这个接口 必须实现它定义的所有抽象方法
 */

#include<QMetaType>
#include<QObject>

class SpecTest : public QObject{

public:

    /*测试结果枚举
     * PASS通过（在结果中呈绿色）
     * FAIL失败 （红色）
     * WARNING警告（黄色）
     * UNKNOWN（不显示颜色）
     */
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
