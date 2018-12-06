#ifndef DEVICELISTENER_H
#define DEVICELISTENER_H

/*
 * @Author liaowenxing
 * 这个类是一个辅助类，用来监听手机的插入和拔出，它依赖于adb
 * 后续考虑将此类写成单例模式，但是会引申一个问题：程序运行期间一直监听
 */

#include<QTimer>
class DeviceListener:public QObject
{
    Q_OBJECT
public:
    DeviceListener();
    bool isListChanged(QStringList newList,QStringList oldList);

    /*
     * 停止监听
     */
    void stop();
signals:

    /*
     * 当连接电脑的手机发生变化时，发出的信号，同时携带所有手机序列号的list
     */
     void deviceChanged(QStringList list);
public slots:
     void checkDevice();
protected:
     void timerEvent(QTimerEvent *event);
private:
    QStringList mDeviceList;
    int mTimerId;
};

#endif // DEVICELISTENER_H
