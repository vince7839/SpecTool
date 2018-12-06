#ifndef SPECTESTTHREAD_H
#define SPECTESTTHREAD_H

/*
 * @Author liaowenxing
 * 工作线程，开始测试时在这个线程中完成
 */

#include<util/specbuilder.h>
#include<QThread>
#include<test/spectest.h>
class SpecTestThread : public QThread
{
    Q_OBJECT
signals:

    /*信号：
     * 向主线程报告测试进度，以便更新进度条
     */
    void testProgress(int value,int max);

    /*
     * 测试完成，将结果发给输出窗口
     */
    void testFinished(QList<SpecTest*> list);
public:
    SpecTestThread(SpecBuilder* builder);
    void run();
private:
    QList<SpecTest*> list;
    SpecBuilder* builder;
};

#endif // SPECTESTTHREAD_H
