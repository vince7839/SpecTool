#include "sdkwidget.h"
#include "ui_sdkwidget.h"
#include<QFile>
#include<QDir>
#include<QFileDialog>
#include<QTextStream>
SdkWidget::SdkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SdkWidget)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("SDK检测结果"));
    ui->tableWidget->setColumnCount(3);
    QStringList header = QStringList()<<QString::fromUtf8("APK包名")
                                     <<QString::fromUtf8("Target SDK要求")<<QString::fromUtf8("实际结果");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->cbox_filter->addItem(QString::fromUtf8("全部"),"ALL");
    ui->cbox_filter->addItem(QString::fromUtf8("MTK应用"),"MTK_ONLY");
    ui->cbox_filter->addItem(QString::fromUtf8("第三方应用"),"VENDOR_ONLY");
    connect(ui->cbox_filter,&QComboBox::currentTextChanged,this,&SdkWidget::doFilter);
    connect(ui->btn_export,&QPushButton::clicked,this,&SdkWidget::saveReport);
}

SdkWidget::~SdkWidget()
{
    delete ui;
}

void SdkWidget::showResult(QList<SpecTest *> list)
{
    mList = list;
    updateTable(list);
}

void SdkWidget::doFilter()
{
    QString type = ui->cbox_filter->currentData().toString();
    updateTable(getTestsByType(type));
}

QList<SpecTest *> SdkWidget::getTestsByType(QString type)
{
    QList<SpecTest*> list;
    if(type == "ALL"){
        list = mList;
    }else if(type == "MTK_ONLY"){
        foreach(SpecTest*test,mList){
            if(test->getName().contains("mediatek")){
                list.append(test); }
        }
    }else if(type == "VENDOR_ONLY"){
        foreach(SpecTest*test,mList){
            if(!test->getName().contains("google") && !test->getName().contains("android") && !test->getName().contains("mediatek")){
                list.append(test); }
        }
    }
    return list;
}

void SdkWidget::saveReport()
{
    QString filename = QFileDialog::getExistingDirectory(this,QString::fromUtf8("报告保存至"),QDir::homePath())
            .append("/SdkReport.txt");
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QTextStream out(&file);
        out<<QString::fromUtf8("不满足要求的应用:\n");
        foreach(SpecTest*test,mList){
            if(test->getStatus() == SpecTest::FAIL){
                out<<QString("%1--->%2\n").arg(test->getName()).arg(test->getResult());
            }
        }
       file.close();
    }
}

void SdkWidget::updateTable(QList<SpecTest *> list)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(list.size());
    for(int i = 0;i < list.size() ; i++){
        SpecTest* test = list.at(i);
        QTableWidgetItem*name = new QTableWidgetItem;
        name->setText(test->getName());
        QTableWidgetItem*expect = new QTableWidgetItem;
        expect->setText(test->getExpect());
        QTableWidgetItem*result = new QTableWidgetItem;
        result->setText(test->getResult());
        if(test->getStatus() == SpecTest::PASS){
            result->setBackgroundColor(Qt::green);
        }else if(test->getStatus() == SpecTest::FAIL){
            result->setBackgroundColor(Qt::red);
        }
        ui->tableWidget->setItem(i,0,name);
        ui->tableWidget->setItem(i,1,expect);
        ui->tableWidget->setItem(i,2,result);
    }
    showMaximized();
}
