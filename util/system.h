#ifndef SYSTEM_H
#define SYSTEM_H

/*
 * 这个类主要用于兼容Windows和Linux平台，对于不同平台间一些差异化的属性，按当前操作系统返回正确的值
 */

#include <QObject>

class System
{   
public:
    System();

    /*
     *返回当前系统的换行符，Windows下是\r\n ，Linux下是\n
     */
    static QString getSeparator();

    /*
     * 此软件的版本号
     */
    static const QString VERSION;
};

#endif // SYSTEM_H
