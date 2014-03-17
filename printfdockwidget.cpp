#include "printfdockwidget.h"
#include "ui_printfdockwidget.h"
#include <QDebug>

PrintfDockWidget *_instance = NULL;

PrintfDockWidget::PrintfDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PrintfDockWidget)
{
    ui->setupUi(this);
}

PrintfDockWidget::~PrintfDockWidget()
{
    delete ui;
}


PrintfDockWidget* PrintfDockWidget::getInstance()
{
    if(_instance == NULL)
        _instance = new PrintfDockWidget();

    return _instance;
}


void PrintfDockWidget::printf(const QString &text)
{
    qDebug() << text;
    ui->textEdit->append(text);
}
