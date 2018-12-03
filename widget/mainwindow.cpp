#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<util/specbuilder.h>
#include<util/spectestthread.h>
#include<widget/resultwidget.h>
#include<QMessageBox>
#include<widget/sdkwidget.h>
#include<QDebug>
#include<util/system.h>
#include<util/executor.h>
#include<widget/infowidget.h>
#include<QSettings>
#include<QApplication>
#include<QDesktopWidget>
#include<QDate>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("规范测试[%1]").arg(System::VERSION));
    listener = new DeviceListener;
    connect(listener,&DeviceListener::deviceChanged,this,&MainWindow::updateDeviceBox);
    connect(ui->btn_start,&QPushButton::clicked,this,&MainWindow::start);
    connect(ui->cbox_test,&QComboBox::currentTextChanged,this,&MainWindow::hideOption);
    ui->cbox_test->addItem(QString::fromUtf8("规范检测"),"SPEC_TEST");
    ui->cbox_test->addItem(QString::fromUtf8("Target SDK检测"),"SDK_TEST");
    this->statusBar()->addWidget(new QLabel(QString::fromUtf8("<font color='red'>Express+测试请保持手机亮屏解锁</font>")));
    QMenu* helpMenu = new QMenu(QString::fromUtf8("帮助"));
    helpMenu->addAction(QString::fromUtf8("关于"),this,&MainWindow::showAbout);
    helpMenu->addAction(QString::fromUtf8("更新历史"),this,&MainWindow::showInfo);
    this->menuBar()->addMenu(helpMenu);
    init();
    moveToCenter(parent);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::prepare()
{
    if(ui->cbox_device->currentText().isEmpty()){
        QMessageBox::warning(this,"",QString::fromUtf8("请选择设备！"));
        return false;
    }
    return true;
}

void MainWindow::updateDeviceBox(QStringList list)
{
    ui->cbox_device->clear();
    ui->cbox_device->addItems(list);
}

void MainWindow::start()
{
    if(!prepare()){
        return;
    }
    QString device  = ui->cbox_device->currentText();
    dialog = new QProgressDialog;
    dialog->setMinimumDuration(0);
    dialog->show();
    dialog->setWindowTitle(QString::fromUtf8("规范测试"));
    dialog->setLabelText(QString::fromUtf8("正在测试..."));
    SpecBuilder*builder = new SpecBuilder();
    QString testType = ui->cbox_test->currentData().toString();
    builder->buildDevice(device)
            ->buildExpress(ui->checkBox_express->isChecked())
       //     ->buildMpType(ui->checkBox_mp->isChecked())
            ->buildTestType(testType);
       //     ->buildGmsRequired(ui->checkBox_gms->isChecked());
    SpecTestThread*thread = new SpecTestThread(builder);
    connect(thread,&SpecTestThread::testProgress,this,&MainWindow::updateProgressDialog);
    if(testType == "SPEC_TEST"){
        ResultWidget* w= new ResultWidget;
        connect(thread,&SpecTestThread::testFinished,w,&ResultWidget::showResult);
    }else{
          SdkWidget*sdkWidget = new SdkWidget;
          connect(thread,&SpecTestThread::testFinished,sdkWidget,&SdkWidget::showResult);
    }
    thread->start();
}

void MainWindow::updateProgressDialog(int value, int max)
{
    dialog->setRange(0,max);
    dialog->setValue(value);
}

void MainWindow::hideOption()
{
    ui->groupBox->setVisible(ui->cbox_test->currentData().toString() == "SPEC_TEST");
}

void MainWindow::showInfo()
{
    InfoWidget*infoWidget = new InfoWidget;
    infoWidget->show();
}

void MainWindow::showAbout()
{
    QMessageBox::information(this,QString::fromUtf8("关于"),QString::fromUtf8("问题反馈QQ:785453694"));
}

void MainWindow::init()
{
    QSettings settings("Sagereal","SpecTool");
    if(settings.value("showAgain").toBool() || settings.value("currentVersion") != System::VERSION){
        showInfo();
    }
    if(settings.value("currentVersion") != System::VERSION){
        settings.setValue("currentVersion",System::VERSION);
    }
    if(!Executor::waitFinish("adb version").startsWith("Android Debug Bridge")){
        QMessageBox::warning(this,QString::fromUtf8("警告"),QString::fromUtf8("检测到adb未配置，功能无法正常使用!"));
    }
}

void MainWindow::moveToCenter(QWidget*parent)
{
    int centerX = 0;
    int centerY = 0;
    if(parent != NULL){
        QPoint center = parent->mapToGlobal(QPoint(parent->width()/2,parent->height()/2));
        centerX = center.x();
        centerY = center.y();
    }else{
        centerX = QApplication::desktop()->width()/2;
        centerY = QApplication::desktop()->height()/2;
    }
    int x = centerX - this->width()/2;
    int y = centerY - this->height()/2;
    this->move(x,y);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    listener->stop();
}
