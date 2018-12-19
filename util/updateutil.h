#ifndef UPDATEUTIL_H
#define UPDATEUTIL_H

#include <QObject>
#include<util/system.h>
#include<QProgressDialog>
#include<QMap>
#include<QDomNodeList>
class UpdateUtil;
class UpdateUtil : public QObject
{
    Q_OBJECT
public:    
    void readServerConfig(System::AccessType type);
    static UpdateUtil *getInstance();
    QString gmsEndDate(QString version);
    QString broadcast();
    QList<QMap<QString,QString> > toolInfo();
    QString priority();
public slots:
    void sendRequest();
    void check();
    void updateSelf(QString ip);
    void onDownloadReply();
    void updateDownloadProgress(qint64 value,qint64 total);
    void launchNewVersion(QString filename);
    void onReadConfigFinish();
    void parseConfig(QString content);
private:
    UpdateUtil();
    QProgressDialog* dialog;
    QMap<QString,QString> config;
    QDomNodeList gmsPackages;
    QList<QMap<QString,QString> > tools;
    static UpdateUtil *sInstance;
    QString availableIP;
};

#endif // UPDATEUTIL_H
