#ifndef NEWPROPERTYDIALOG_H
#define NEWPROPERTYDIALOG_H

#include <QDialog>
#include "propertywidget.h"

namespace Ui {
class NewPropertyDialog;
}

class NewPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewPropertyDialog(View *view);
    ~NewPropertyDialog();

    //PropertyWidget browser;
private:
    Ui::NewPropertyDialog *ui;
};

#endif // NEWPROPERTYDIALOG_H
