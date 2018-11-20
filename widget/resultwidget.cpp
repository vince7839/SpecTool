#include "resultwidget.h"
#include "ui_resultwidget.h"
#include<test/spectest.h>
#include<test/proptest.h>
#include<test/packagetest.h>
#include<util/specutil.h>
#include<QDebug>
#include<QFileDialog>
#include<QFile>
#include<util/system.h>
#include<QMessageBox>
#include<QMenu>
#include<QProgressBar>
#include<QApplication>
#include<QClipboard>
#include<QDate>
#include<QHostInfo>

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
    ui->label->setText(QString::fromUtf8("<font color='green'>绿色PASS</font>，<font color='red'>红色FAIL</font>，<font color='yellow'>黄色</font>和其他视项目而定"));
    setWindowTitle(QString::fromUtf8("规范测试结果"));
    ui->tableWidget->setColumnCount(3);
    QStringList header = QStringList()<<QString::fromUtf8("name")
                                     <<QString::fromUtf8("期望结果")<<QString::fromUtf8("实际结果");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->btn_export,&QPushButton::clicked,this,&ResultWidget::saveReport);
    connect(ui->tableWidget,&QTableWidget::customContextMenuRequested,this,&ResultWidget::showMenu);
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::showResult(QList<SpecTest *> list)
{
    qDebug()<<"ResultWidget::showResult:"<<list.size();
    mList = list;
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
        }else if(test->getStatus() == SpecTest::WARNING){
            result->setBackgroundColor(Qt::yellow);
        }
        ui->tableWidget->setItem(i,0,name);
        ui->tableWidget->setItem(i,1,expect);
        ui->tableWidget->setItem(i,2,result);
    }
    showMaximized();
}

void ResultWidget::saveReport()
{
    QFile reportTemplate(":/resource/report_html");
    if(!reportTemplate.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("无法打开报告模板！"));
        return;
    }
    QString content = reportTemplate.readAll();
    reportTemplate.close();
    int passCount = 0;
    int failCount = 0;
    int warningCount = 0;
    QString table;
    foreach(SpecTest*test,mList){
        QString color;
        if(test->getStatus() == SpecTest::FAIL){
            failCount++;
            color = "red";
        }else if(test->getStatus() == SpecTest::WARNING){
            warningCount++;
            color = "yellow";
        }else if(test->getStatus() == SpecTest::PASS){
            passCount++;
            color = "green";
        }
        QString row = QString("<tr><td>%1</td><td>%2</td><td  bgcolor=%3>%4</td></tr>\n")
                .arg(test->getName()).arg(test->getExpect()).arg(color).arg(test->getResult());
        table.append(row);
    }
    content.replace("<SpecTool:toolVersion>",System::VERSION)
            .replace("<SpecTool:testTime>",QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
            .replace("<SpecTool:hostInfo>",QHostInfo::localHostName())
            .replace("<SpecTool:passCount>",QString::number(passCount))
            .replace("<SpecTool:failCount>",QString::number(failCount))
            .replace("<SpecTool:warningCount>",QString::number(warningCount))
            .replace("<SpecTool:totalSize>",QString::number(mList.size()))
            .replace("<SpecTool:resultTable>",table);
    QString dirPath = QFileDialog::getExistingDirectory(this,QString::fromUtf8("报告保存至"),QDir::homePath());
    if(dirPath.isEmpty()){
        return;
    }
    QString filename = dirPath.append(QString::fromUtf8("/基础排查报告.html"));
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QTextStream in(&file);
        in.setCodec("UTF-8");
        in<<content;
        file.close();
        QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("报告已保存至%1").arg(filename));
    }
}

void ResultWidget::showMenu(QPoint pos)
{
    QTableWidgetItem*currentItem = ui->tableWidget->itemAt(pos);
    if(currentItem == NULL){
        qDebug()<<"item at cusor is null";
        return;
    }
    ui->tableWidget->setCurrentItem(currentItem);
    QMenu*menu=new QMenu;
    QAction*retestAction=new QAction(QString::fromUtf8("复测此项"),menu);
    QAction*copyAction = new QAction(QString::fromUtf8("复制测试指令"),menu);
    connect(retestAction,&QAction::triggered,this,&ResultWidget::retestItem);
    connect(copyAction,&QAction::triggered,this,&ResultWidget::copyCmd);
    menu->addAction(copyAction);
    menu->addAction(retestAction);
    if(mList.at(currentItem->row())->getCmd().isEmpty()){
        copyAction->setEnabled(false);
    }
    menu->exec(QCursor::pos());
}

void ResultWidget::retestItem()
{
    int row = ui->tableWidget->currentRow();
    qDebug()<<"retest"<<row;
    if(row >= 0 && row < mList.size()){
        setCursor(Qt::WaitCursor);
        mList.at(row)->run();
        showResult(mList);
        setCursor(Qt::ArrowCursor);
    }
}

void ResultWidget::copyCmd()
{
    QString cmd = mList.at(ui->tableWidget->currentRow())->getCmd();
    QApplication::clipboard()->setText(cmd);
}

