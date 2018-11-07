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
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("规范测试"));
    listener = new DeviceListener;
    connect(listener,&DeviceListener::deviceChanged,this,&MainWindow::updateDeviceBox);
    connect(ui->btn_start,&QPushButton::clicked,this,&MainWindow::start);
    connect(ui->cbox_test,&QComboBox::currentTextChanged,this,&MainWindow::hideOption);
    ui->cbox_phoneType->addItem(QString::fromUtf8("要过GMS认证的Smart Phone"),SpecBuilder::GMS_REQUIRED);
    ui->cbox_phoneType->addItem(QString::fromUtf8("不过GMS认证的Smart Phone"),SpecBuilder::GMS_NOT_REQUIRED);
    ui->cbox_phoneType->addItem(QString::fromUtf8("Smart Feature Phone"),SpecBuilder::SMART_FEATURE_PHONE);
    ui->cbox_test->addItem(QString::fromUtf8("规范检测"),"SPEC_TEST");
    ui->cbox_test->addItem(QString::fromUtf8("Target SDK检测"),"SDK_TEST");
    this->statusBar()->showMessage(QString::fromUtf8("Express+测试请保持手机亮屏解锁"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDeviceBox(QStringList list)
{
    ui->cbox_device->clear();
    ui->cbox_device->addItems(list);
}

void MainWindow::start()
{
    QString device  = ui->cbox_device->currentText();
    if(device.isEmpty()){
        QMessageBox::warning(this,"",QString::fromUtf8("请选择设备！"));
        return;
    }
    dialog = new QProgressDialog;
    dialog->setMinimumDuration(0);
    dialog->show();
    dialog->setWindowTitle(QString::fromUtf8("规范测试"));
    dialog->setLabelText(QString::fromUtf8("正在测试..."));
    SpecBuilder*builder = new SpecBuilder();
    QString testType = ui->cbox_test->currentData().toString();
    builder->buildDevice(device)
            ->buildExpress(ui->checkBox_express->isChecked())
            ->buildMpType(ui->checkBox_mp->isChecked())
            ->buildTestType(testType)
            ->buildPhoneType(ui->cbox_phoneType->currentData().toInt());
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    listener->stop();
}
