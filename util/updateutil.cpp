#include "updateutil.h"
#include<QNetworkReply>
#include<QMessageBox>
#include<QProcess>
#include<util/networkutil.h>
#include<QApplication>
#include<QFile>
#include<util/executor.h>
#include<QObject>
#include<QProgressDialog>
#include<QDir>
#include<QThread>
#include<QDebug>
#include<QDomDocument>
#include<QDomNode>
#include<widget/advicewidget.h>

UpdateUtil *UpdateUtil::sInstance = nullptr;

UpdateUtil::UpdateUtil()
{

}

void UpdateUtil::readServerConfig(System::AccessType type)
{
    NetworkUtil*net = NetworkUtil::getInstance();
    QNetworkReply*reply = net->get(System::configUrl(type));
    connect(reply,&QNetworkReply::finished,this,&UpdateUtil::onReadConfigFinish);
}

UpdateUtil *UpdateUtil::getInstance()
{
    if(sInstance == nullptr){
        sInstance = new UpdateUtil;
    }
    return sInstance;
}

QString UpdateUtil::gmsEndDate(QString version)
{
    QString value;
    for(int i = 0 ; i < gmsPackages.size() ; i++ ){
        QDomNode node = gmsPackages.at(i);
        if(node.attributes().namedItem("version").nodeValue() == version){
            value = node.attributes().namedItem("end").nodeValue();
        }
    }
    return value;
}

QString UpdateUtil::broadcast()
{
    return config.value("broadcast");
}

QList<QMap<QString, QString> > UpdateUtil::toolInfo()
{
    return tools;
}

QString UpdateUtil::priority()
{
    return config.value("priority");
}

void UpdateUtil::sendRequest()
{
    readServerConfig(System::TYPE_INTERNAL);
    readServerConfig(System::TYPE_EXTERNAL);
}

void UpdateUtil::check()
{
    QString lastedVersion = config.value("lastedVersion");
    if(!lastedVersion.isEmpty() && lastedVersion != System::VERSION){
        QMessageBox::information(0,QString::fromUtf8("提示"),QString::fromUtf8("检测到服务器有新版本，即将开始更新"));
        dialog = new QProgressDialog;
        dialog->show();
        dialog->setRange(0,0);
        updateSelf(availableIP);
    }else{
        AdviceWidget*w=new AdviceWidget;
        w->show();
    }
}

void UpdateUtil::updateSelf(QString ip)
{
    NetworkUtil*net = NetworkUtil::getInstance();
    QNetworkReply*reply = net->get(System::updateUrl(ip));
    connect(reply,&QNetworkReply::finished,this,&UpdateUtil::onDownloadReply);
    connect(reply,&QNetworkReply::downloadProgress,this,&UpdateUtil::updateDownloadProgress);
}

void UpdateUtil::onDownloadReply()
{
    if(dialog != nullptr){
        dialog->close();
    }
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    if(reply->bytesAvailable() <= 0){
        return;
    }
    /*
     * windows系统无法删除打开的程序，所以先改名为old.exe，启动新程序后删除它
     * linux则直接删除老版本
     */
    QString currentFileName = qApp->applicationFilePath();
    if(System::isWindows()){
        QString changeName = qApp->applicationDirPath().append("/old.exe");
        QFile::remove(changeName);
        QFile::rename(currentFileName,changeName);
    }else{
        if(!QFile::remove(currentFileName)){
            QMessageBox::information(0,QString::fromUtf8("提示"),QString::fromUtf8("删除老版本失败，你可以手动删除"));
        }
    }
    QString newName = qApp->applicationDirPath().append(System::applicationName());
    QFile target(newName);
    if(target.open(QIODevice::ReadWrite|QIODevice::Truncate)){
        QByteArray byteArray = reply->readAll();
        target.write(byteArray);
        target.close();
        QMessageBox::information(0,QString::fromUtf8("提示"),QString::fromUtf8("自动更新完成，现在重启"));
        launchNewVersion(newName);
    }else{
        QMessageBox::warning(0,QString::fromUtf8("错误"),QString::fromUtf8("无法创建文件%1").arg(newName));
    }
}

void UpdateUtil::updateDownloadProgress(qint64 value, qint64 total)
{
    qDebug()<<value<<total;
    dialog->setLabelText(QString::fromUtf8("已接收数据%1kb").arg(value));
}

void UpdateUtil::launchNewVersion(QString filename)
{
    if(!System::isWindows()){
        QString output = Executor::waitFinish(QString("chmod 777 %1").arg(filename));
        if(!output.isEmpty()){
            QMessageBox::information(0,QString::fromUtf8("提示"),QString::fromUtf8("自启动失败，请手动重启！"));
        }
    }
    QProcess::startDetached(filename);
    qApp->quit();
}

void UpdateUtil::onReadConfigFinish()
{
    qDebug()<<"onReadConfigFinish";
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    QString ip = reply->url().host();
    if(reply->error() == QNetworkReply::NoError){
        QString content = reply->readAll().trimmed();
        availableIP = ip;
        if(!content.isEmpty()){
            parseConfig(content);
            check();
        }
    }else{
        static bool internalFail = false;
        static bool externalFail = false;
        internalFail = ip == System::serverIP(System::TYPE_INTERNAL);
        externalFail = ip == System::serverIP(System::TYPE_EXTERNAL);
        if(internalFail && externalFail){
            QMessageBox::warning(0,QString::fromUtf8("警告")
                                 ,QString::fromUtf8("自动检查更新失败，无法连接到服务器%1").arg(ip));
            sendRequest();
        }
    }
}

void UpdateUtil::parseConfig(QString content)
{
    QDomDocument doc;
    doc.setContent(content);
    QDomNode rootNode = doc.namedItem("SpecTool");
    QString lastedVersion = rootNode.namedItem("Version").toElement().text();
    QString broadcast = rootNode.namedItem("Broadcast").toElement().text();
    QString priority = rootNode.namedItem("Broadcast").attributes().namedItem("priority").nodeValue();
    gmsPackages = doc.elementsByTagName("package");

    /*
     * 更新服务器信息
     */
    config.clear();
    config.insert("broadcast",broadcast.trimmed());
    config.insert("lastedVersion",lastedVersion);
    config.insert("priority",priority);
    QDomNodeList toolNodes = doc.elementsByTagName("tool");

    /*
     * 更新工具信息
     */
    tools.clear();
    for(int i = 0 ; i < toolNodes.size() ; i++){
        QMap<QString,QString> map;
        QDomNode node = toolNodes.at(i);
        QString name = node.attributes().namedItem("name").nodeValue();
        QString start = node.attributes().namedItem("start").nodeValue();
        QString end = node.attributes().namedItem("end").nodeValue();
        map.insert("name",name);
        map.insert("start",start);
        map.insert("end",end);
        tools.append(map);
    }
}
