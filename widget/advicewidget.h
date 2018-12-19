#ifndef ADVICEWIDGET_H
#define ADVICEWIDGET_H

#include <QWidget>
#include<widget/popwidget.h>
namespace Ui {
class AdviceWidget;
}

class AdviceWidget : public PopWidget
{
    Q_OBJECT

public:
    explicit AdviceWidget(QWidget *parent = 0);
    ~AdviceWidget();
    void appendFeature();
    void appendAdvice();
    void appendToolInfo();
private:
    Ui::AdviceWidget *ui;
};

#endif // ADVICEWIDGET_H
