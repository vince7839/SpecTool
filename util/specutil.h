#ifndef SPECUTIL_H
#define SPECUTIL_H

/*
 * @Author liaowenxing
 * 这是一个工具类、单例模式 用来获取手机的各项属性或者做一些判断
 */

#include <QMap>

class SpecUtil
{    
private :
    SpecUtil(QString device);
public:

    /*
     * 单例模式获取对象的静态方法，保证应用运行期间始终只有一个对象
     */
    static SpecUtil* getInstance(QString device);

    /*
     * 相当于adb shell getprop命令
     */
    QString getProp(QString prop);

    /*
     * 将fingerprint按/分割，并返回第index个字段
     */
    QString fingerprintAttr(int index);

    /*
     * first API Level
     */
    QString getApiLevel();

    /*
     * 软件应有的first API Level
     */
    QString expectApiLevel();

    /*
     * 是否有Express+项目应该写的Feature
     */
    bool isExpress();

    /*
     * 软件是否Go版本
     */
    bool isGoVersion();

    /*
     * ram是否 < 1G
     */
    bool ramLimit();

    /*
     * 返回安全patch至今的天数
     */
    int patchDayCount();

    /*
     * 是否含有某个Feature
     */
    bool hasFeature(QString feature);

    /*
     * 是否安装了某个APK
     */
    bool hasPackage(QString package);

    /*
     *获取手机数据库中某个设置项的值
     */
    QString getSetting(QString type,QString key);

    /*
     * 获取当前手机序列号
     */
    QString getCurrentDevice();

    /*
     * 有没有ro值中含有公司名称
     */
    QString companyLimit();

    /*
     * 获取手机中所有APK
     */
    QStringList getPackages();

    /*
     * 获取手机data区的大小，返回kb数
     */
    int dataSize();

    /*
     * 返回system区剩余可用空间
     */
    int systemAvailable();
private:
    static QString device;
    static SpecUtil* sInstance;
    QStringList packages;
    QStringList features;
    QMap<QString,QString> props;
};

#endif // SPECUTIL_H
