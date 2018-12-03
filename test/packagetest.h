#ifndef PACKAGETEST_H
#define PACKAGETEST_H

/*
 *@Author liaowenxing
 *  这个类用于判断手机中是否安装了指定的apk
 */

#include<test/spectest.h>
#include<util/specutil.h>
class PackageTest : public SpecTest
{
public:
    static const QString CHROME ;
    static const QString PLAY_STORE ;
    static const QString MAPS_GO ;
    static const QString MAPS;
    static const QString GMAIL_GO ;
    static const QString GMAIL ;
    static const QString GOOGLE_GO ;
    static const QString GOOGLE;
    static const QString YOUTUBE_GO;
    static const QString YOUTUBE;
    static const QString ASSITANT_GO;
    static const QString DRIVE;
    static const QString PLAY_MUSIC;
    static const QString PLAY_MOVIES;
    static const QString PHOTOS;
    static const QString DUO;
    static const QString LIVE_PAPER;
    static const QString FACE_LOCK;
    static const QString LOVELY_FONT;
    static const QString NAV_GO;
    static const QString GOOGLE_MESSAGE;
    static const QString QUICK_SEARCH_BOX;
    PackageTest(QString device,QString package,bool expect = true,QString name = "");
    void run();
    QString getName();
    QString getExpect();
    QString getResult();
    Status getStatus();
    void initNames();
private:
    QString device;
    QString package;
    QString result;
    QString name;
    bool expect;
    Status status;
    SpecUtil*util;
    QMap<QString,QString> names;
};

#endif // PACKAGETEST_H
