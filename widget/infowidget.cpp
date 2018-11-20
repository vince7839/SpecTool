#include "infowidget.h"
#include "ui_infowidget.h"
#include<QFile>
#include<QTextStream>
#include<QSettings>

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
    setWindowTitle(QString::fromUtf8("更新历史"));
    setWindowModality(Qt::ApplicationModal);
    QSettings settings("Sagereal","SpecTool");
    ui->checkBox->setChecked(!settings.value("showAgain").toBool());
    connect(ui->btn_close,&QPushButton::clicked,this,&InfoWidget::close);
    connect(ui->checkBox,&QCheckBox::toggled,this,&InfoWidget::showAgain);
    QFile file(":/resource/info_txt");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&file);
        in.setCodec("UTF-8");
        while(!in.atEnd()){
            ui->textBrowser->append(in.readLine());
        }
       // ui->textBrowser->setText(in.readAll());
        file.close();
    }
}

InfoWidget::~InfoWidget()
{
    delete ui;
}
