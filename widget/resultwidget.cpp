#include "resultwidget.h"
#include "ui_resultwidget.h"
#include<test/spectest.h>
#include<test/proptest.h>
#include<test/packagetest.h>
#include<util/specutil.h>
#include<QDebug>

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
    ui->label->setText(QString::fromUtf8("<font color='green'>绿色PASS</font>，<font color='red'>红色FAIL</font>，其他视项目而定，需要自己check"));
    setWindowTitle(QString::fromUtf8("规范测试结果"));
    ui->tableWidget->setColumnCount(3);
    QStringList header = QStringList()<<QString::fromUtf8("name")
                                     <<QString::fromUtf8("期望结果")<<QString::fromUtf8("实际结果");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::showResult(QList<SpecTest *> list)
{
    qDebug()<<"ResultWidget::showResult:"<<list.size();
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

