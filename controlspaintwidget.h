#ifndef CONTROLSPAINTWIDGET_H
#define CONTROLSPAINTWIDGET_H

#include <QWidget>

namespace Ui {
class ControlsPaintWidget;
}

class ControlsPaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlsPaintWidget(QWidget *parent, QString pixmap, QString name, QString describe);
    ~ControlsPaintWidget();

    QString className;
    void mouseMoveEvent(QMouseEvent *event);
private:
    Ui::ControlsPaintWidget *ui;
    QPixmap pixmap;
};

#endif // CONTROLSPAINTWIDGET_H
