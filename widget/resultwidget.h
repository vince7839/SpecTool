#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H
#include <QWidget>
#include<test/spectest.h>

namespace Ui {
    class ResultWidget;
}
class ResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResultWidget(QWidget *parent = 0);
    ~ResultWidget();
public slots:
    void showResult(QList<SpecTest*> list);
private:
    Ui::ResultWidget *ui;
    QString device;
    QList<SpecTest*> list;
};

#endif // RESULTWIDGET_H
