#ifndef BUILDRESULTWIDGET_H
#define BUILDRESULTWIDGET_H

#include <QWidget>
#include<util/devicelistener.h>
#include<QProgressDialog>
#include<test/spectest.h>
namespace Ui {
class BuildSpecWidget;
}

class BuildSpecWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuildSpecWidget(QWidget *parent = 0);
    ~BuildSpecWidget();
public slots:
    void updateDeviceBox(QStringList list);
    void start();
    void updateProgressDialog(int value,int max);
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::BuildSpecWidget *ui;
    DeviceListener* listener ;
    QProgressDialog* dialog;
};

#endif // BUILDRESULTWIDGET_H
