#include "controlspaintwidget.h"
#include "ui_controlspaintwidget.h"
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

ControlsPaintWidget::ControlsPaintWidget(QWidget *parent, QString pixmap,QString name, QString describe) :
    QWidget(parent),
    ui(new Ui::ControlsPaintWidget)
{
    ui->setupUi(this);
    this->pixmap.load(pixmap);
    ui->image->setPixmap(this->pixmap);
    ui->name->setText(name);
    ui->describe->setText(describe);

    className = name;
}

ControlsPaintWidget::~ControlsPaintWidget()
{
    delete ui;
}

void ControlsPaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(className);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->exec();
        delete drag;
    }
    QWidget::mouseMoveEvent(event);
}
