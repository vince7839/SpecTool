#include "infowidget.h"
#include "ui_infowidget.h"
#include<QFile>
#include<QTextStream>
#include<QSettings>
#include<util/system.h>
#include<QDebug>
void InfoWidget::showAgain(bool checked)
{
    QSettings settings("Sagereal","SpecTool");
    settings.setValue("showAgain",!checked);
}

InfoWidget::InfoWidget(QWidget *parent) :
    PopWidget(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Tool);
    setWindowTitle(QString::fromUtf8("更新历史"));
    setWindowModality(Qt::ApplicationModal);
    QSettings settings("Sagereal","SpecTool");
    ui->checkBox->setChecked(!settings.value("showAgain").toBool());
    connect(ui->btn_close,&QPushButton::clicked,this,&InfoWidget::close);
    connect(ui->checkBox,&QCheckBox::toggled,this,&InfoWidget::showAgain);
    //    QFile file(":/resource/info_txt");
    //    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
    //        QTextStream in(&file);
    //        in.setCodec("UTF-8");
    //        while(!in.atEnd()){
    //            ui->textBrowser->append(in.readLine());
    //        }
    //       // ui->textBrowser->setText(in.readAll());
    //        file.close();
    //    }
    QList<QMap<QString,QString> > history = System::versionHistory();
    for(int i = 0;i < history.size();i++){
        QMap<QString,QString> map = history.at(i);
        ui->textBrowser->append(QString::fromUtf8("<font color='green'>[%1]</font>%2")
                                .arg(map.value("time")).arg(i == 0 ? "<font color='red'>*</font>" : ""));
        ui->textBrowser->append(map.value("content").trimmed());
    }

    ui->textBrowser->moveCursor(QTextCursor::Start);
}

InfoWidget::~InfoWidget()
{
    delete ui;
}
