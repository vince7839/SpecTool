#ifndef SPECTEST_H
#define SPECTEST_H

/*
 * @Author liaowenxing
 * 这个类是一个纯虚类（相当于Java中的抽象类）它 不能被实例化 它是所有测试类的父类 要实现这个接口 必须实现它定义的所有抽象方法
 */

#include<QMetaType>
#include<QObject>

class SpecTest : public QObject{

public:

    /*
     * 测试结果枚举
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

    /*
     *这个函数会在测试期间被调用，它会被放在子线程中执行，所以在这个函数中写测试所需的核心部分和一些耗时的任务
     */
    virtual void run() = 0;

    /*
     * 下面的这些getter方法将会在显示结果的窗口使用
     * 返回测试项的名称，将会显示在输出窗口的“名称”列
     */
    virtual QString getName() = 0;

    /*
     * 返回这个测试项的期望结果，将会被显示在输出窗口的“期望结果”列
     */
    virtual QString getExpect() = 0;

    /*
     * 返回这个测试的输出结果，将会被显示在输出窗口的“实际结果”列
     */
    virtual QString getResult() = 0;

    /*
     * 返回这个测试的结果，PASS还是FAIL等，决定该条测试项的颜色
     * @see enum Status
     */
    virtual Status getStatus() = 0;

    /*
     * 如果这个测试类可以通过一行代码执行，那么你可以返回这个命令，如果返回字符串不为空，那么输出窗口会显示“复制测试指令”菜单项
     */
    virtual QString getCmd() {return "";}
};
Q_DECLARE_METATYPE(SpecTest*)
#endif // SPECTEST_H
