#ifndef NEWCONTROLDIALOG_H
#define NEWCONTROLDIALOG_H

#include <QDialog>

namespace Ui {
class NewControlDialog;
}

#include "gamescene.h"
#include "propertywidget.h"

class NewControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewControlDialog(GameScene *scene);
    ~NewControlDialog();

    View* buildView();
    QString parentname;
private slots:
    void propertyChange(QtProperty* property,QVariant variant);
private:
    Ui::NewControlDialog *ui;
   // PropertyWidget browser;
    GameScene *scene;
};

#endif // NEWCONTROLDIALOG_H
