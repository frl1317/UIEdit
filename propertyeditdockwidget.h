#ifndef PROPERTYEDITDOCKWIDGET_H
#define PROPERTYEDITDOCKWIDGET_H

#include <QDockWidget>
#include "propertywidget.h"

namespace Ui {
class PropertyEditDockWidget;
}

class PropertyEditDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit PropertyEditDockWidget(QWidget *parent = 0);
    ~PropertyEditDockWidget();

    PropertyWidget *propertywidget;
private:
    Ui::PropertyEditDockWidget *ui;
};

#endif // PROPERTYEDITDOCKWIDGET_H
