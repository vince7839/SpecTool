#ifndef SPECBUILDER_H
#define SPECBUILDER_H

/*
 * @Author liaowenxing
 * 这个类使用建造者模式，通过不同的条件来为手机构建测试内容
 */

#include <QObject>
#include<test/spectest.h>
#include<util/specutil.h>
class SpecBuilder
{
public:
    SpecBuilder();

    /*
     * 构建手机的序列号
     */
    SpecBuilder*buildDevice(QString device);

    /*
     * 是否Express+项目
     */
    SpecBuilder*buildExpress(bool isExpress);

    /*
     * 测试类型 规范 or SDK
     */
    SpecBuilder*buildTestType(QString testType);

    /*
     * 构建，返回测试项的List
     */
    QList<SpecTest*> create();

    /*
     * 所有项目都需要的测试项
     */
    void addCommonTests();

    /*
     * Go项目独有的测试项
     */
    void addGoTests();

    /*
     * 非Go项目独有的测试项
     */
    void addNonGoTests();

    /*
     * Express+项目的测试项
     */
    void addExpressTests();
    void addAndroid8();
    void addEeaTests();
private:
    QString device;
    QString testType;
    bool isExpress = false;
    QList<SpecTest*> list;
    SpecUtil*util;
};

#endif // SPECBUILDER_H
