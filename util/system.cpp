#include "system.h"
#include<QDebug>
#include<QDomDocument>
#include<QDomNode>
#include<QFile>
const QString System::VERSION = "2018-12-19";
System::System()
{

}

QString System::getSeparator()
{
    QString separator = "\n";
    if(isWindows()){
        separator = "\r\n";
    }
    return separator;
}

bool System::isWindows()
{
    return QSysInfo::productType().contains("windows",Qt::CaseInsensitive);
}

QString System::versionUrl(AccessType type)
{
    QString url = QString("http://%1:8007/gitweb/?p=AutoTool.git;a=blob_plain;f=VERSION;hb=HEAD").arg(serverIP(type));
    return url;
}

QString System::updateUrl(QString ip)
{
    QString url = "http://%1:8007/gitweb/?p=GMS_Resource.git;a=blob_plain;f=SpecTool/%2;hb=HEAD";
    QString os = "Linux/SpecTool";
    if(isWindows()){
       os = "Windows/SpecTool/SpecTool.exe";
    }
    url = url.arg(ip).arg(os);
    qDebug()<<url;
    return url;
}

QString System::serverIP(AccessType type)
{
    if(type == TYPE_EXTERNAL){
        return "122.227.143.251";
    }else{
        return "192.168.3.76";
    }
}

QString System::configUrl(AccessType type)
{
    QString url = "http://%1:8007/gitweb/?p=GMS_Resource.git;a=blob_plain;f=SpecTool/Config.xml;hb=HEAD";
    url = url.arg(serverIP(type));
    qDebug()<<url;
    return url;
}

QList<QMap<QString, QString> > System::versionHistory()
{
    QDomDocument doc;
    QFile file(":/resource/info_xml");
    doc.setContent(&file);
    QDomNodeList list = doc.elementsByTagName("publish");
    QList<QMap<QString,QString> >history;
    for(int i = 0; i < list.size() ; i++){
        QDomNode node = list.at(i);
        QString time = node.attributes().namedItem("time").nodeValue();
        QString content = node.toElement().text().trimmed();
        QMap<QString,QString> map;
        map.insert("time",time);
        map.insert("content",content);
        history.append(map);
    }
    return history;
}

QString System::applicationName()
{
    return System::isWindows() ? "/SpecTool.exe" : "/SpecTool";
}
