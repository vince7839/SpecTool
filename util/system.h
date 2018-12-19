#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>

class System
{   
public:
    /*
     * 重庆和奉化对服务器的访问地址不同，CQ代表重庆
     */
    enum AccessType{
        TYPE_INTERNAL,
        TYPE_EXTERNAL
    };
    System();
    static QString getSeparator();
    static const QString VERSION;
    static bool isWindows();
    static QString versionUrl(AccessType type);
    static QString updateUrl(QString ip);
    static QString serverIP(AccessType type);
    static QString configUrl(AccessType type);
    static QList<QMap<QString,QString> > versionHistory();
};

#endif // SYSTEM_H
