#include "networkutil.h"
#include<QNetworkReply>
#include<QObject>
#include<QXmlStreamReader>
#include<QDebug>

NetworkUtil* NetworkUtil::sInstance = nullptr;
NetworkUtil::NetworkUtil()
{
    manager = new QNetworkAccessManager;
}

NetworkUtil *NetworkUtil::getInstance()
{
    if(sInstance == nullptr){
        sInstance = new NetworkUtil();
    }
    return sInstance;
}

QNetworkReply* NetworkUtil::get(QString url,QMap<QString,QString>params)
{    
    qDebug()<<params;
    if(!params.isEmpty()){
        foreach(QString key,params.keys()){
            url.append(QString("&%1=%2").arg(key).arg(params.value(key)));
        }
    }
    qDebug()<<"[NetworkUtil]get:"+url;
    request.setUrl(QUrl(url));
    QNetworkReply*r =  manager->get(request);
    return r;
}

QNetworkReply *NetworkUtil::post(QString url)
{    
}
