#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include<widget/popwidget.h>
namespace Ui {
class InfoWidget;
}

class InfoWidget : public PopWidget
{
    Q_OBJECT
public slots:
    void showAgain(bool checked);
public:
    explicit InfoWidget(QWidget *parent = 0);
    ~InfoWidget();

private:
    Ui::InfoWidget *ui;
};

#endif // INFOWIDGET_H
