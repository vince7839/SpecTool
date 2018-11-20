#ifndef SDKWIDGET_H
#define SDKWIDGET_H

#include <QWidget>
#include<test/spectest.h>
namespace Ui {
class SdkWidget;
}

class SdkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SdkWidget(QWidget *parent = 0);
    ~SdkWidget();
    void showResult(QList<SpecTest*> list);
    void doFilter();
    QList<SpecTest*> typedList(QString type);
    QList<SpecTest*> searchedList(QList<SpecTest*> list);
    void saveReport();
    void updateTable(QList<SpecTest*> list);
private:
    Ui::SdkWidget *ui;
    QList<SpecTest*> mList;
};

#endif // SDKWIDGET_H
