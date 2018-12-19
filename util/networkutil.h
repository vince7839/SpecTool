#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H

/*
 * 这个类是一个单例模式的网络调度中心，使用它提供的方法来访问网络
 */

#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
class NetworkUtil;
class NetworkUtil:public QObject
{
Q_OBJECT
public:
    static NetworkUtil* getInstance();
    QNetworkReply* get(QString url,QMap<QString,QString> params = QMap<QString,QString>());
    QNetworkReply* post(QString url);
private:
    NetworkUtil();
    static NetworkUtil* sInstance;
    QNetworkAccessManager* manager;
    QNetworkRequest request;
};

#endif // NETWORKUTIL_H
