#ifndef PRINTFDOCKWIDGET_H
#define PRINTFDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class PrintfDockWidget;
}

class PrintfDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    static PrintfDockWidget* getInstance();

    explicit PrintfDockWidget(QWidget *parent = 0);
    ~PrintfDockWidget();

    void printf(const QString &text);


private:
    Ui::PrintfDockWidget *ui;
};

#endif // PRINTFDOCKWIDGET_H
