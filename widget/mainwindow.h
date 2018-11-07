#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<util/devicelistener.h>
#include<QProgressDialog>

namespace Ui {
    class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void updateDeviceBox(QStringList list);
    void start();
    void updateProgressDialog(int value,int max);
    void hideOption();
protected:
    void closeEvent(QCloseEvent *event);
private:
    DeviceListener* listener ;
    QProgressDialog* dialog;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
