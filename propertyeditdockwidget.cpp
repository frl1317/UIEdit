#include "propertyeditdockwidget.h"
#include "ui_propertyeditdockwidget.h"

PropertyEditDockWidget::PropertyEditDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PropertyEditDockWidget)
{
    ui->setupUi(this);

    //propertywidget = new PropertyWidget();
}

PropertyEditDockWidget::~PropertyEditDockWidget()
{
   // delete propertywidget;
    delete ui;
}
