#include "advicewidget.h"
#include "ui_advicewidget.h"
#include<QDebug>
#include<util/system.h>
#include<util/updateutil.h>
#include<QDate>
AdviceWidget::AdviceWidget(QWidget *parent) :
    PopWidget(parent),
    ui(new Ui::AdviceWidget)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    ui->textBrowser->setWordWrapMode(QTextOption::NoWrap);
    connect(ui->btn_close,&QPushButton::clicked,this,&AdviceWidget::close);
    appendFeature();
    appendAdvice();
    appendToolInfo();
}

AdviceWidget::~AdviceWidget()
{
    delete ui;
}

void AdviceWidget::appendFeature()
{
    ui->textBrowser->append(QString::fromUtf8("<font color='black' size='4'>更新要点</font>"));
    QList<QMap<QString,QString> > history = System::versionHistory();
    if(!history.isEmpty()){
        QMap<QString,QString> map = history.first();
        ui->textBrowser->append(QString("<font color='green'>[%1]</font>").arg(map.value("time")));
        ui->textBrowser->append(map.value("content"));
    }
    ui->textBrowser->append("");
}

void AdviceWidget::appendAdvice()
{
    ui->textBrowser->append(QString::fromUtf8("<font color='black' size='4'>GMS重要信息知会</font>"));
    QString priority = UpdateUtil::getInstance()->priority();
    ui->textBrowser->append(UpdateUtil::getInstance()->broadcast());
    ui->textBrowser->append("");
}

void AdviceWidget::appendToolInfo()
{
    ui->textBrowser->append(QString::fromUtf8("<font color='black' size='4'>工具期限</font>"));
    QList<QMap<QString,QString> > tools = UpdateUtil::getInstance()->toolInfo();
    for(int i = 0;i < tools.size() ; i ++){
        QMap<QString,QString> map = tools.at(i);
        QDate dueDate = QDate::fromString(map.value("end"),"yyyy-MM-dd");
        int days = 0;
        if(!dueDate.isNull())
        {
            days = QDate::currentDate().daysTo(dueDate);
        }
        ui->textBrowser->append(QString::fromUtf8("%1----截止日期%2----<font color='%3'>剩余%4天</font>")
                                .arg(map.value("name")).arg(map.value("end")).arg(days < 20 ? "red" : "green").arg(days));
    }
}
