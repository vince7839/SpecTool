#ifndef POPWIDGET_H
#define POPWIDGET_H

#include <QWidget>

class PopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PopWidget(QWidget *parent = 0);    
    void moveToCenter();
public slots:
    void show();
private:
    QWidget* parent;
};

#endif // POPWIDGET_H
