#include "newpropertydialog.h"
#include "ui_newpropertydialog.h"
#include "view.h"

NewPropertyDialog::NewPropertyDialog(View *view):
    ui(new Ui::NewPropertyDialog)
{
    ui->setupUi(this);

    QVBoxLayout *l =  (QVBoxLayout *)layout();
    //l->addWidget( &browser, -1, Qt::AlignVCenter);
    l->addWidget(ui->buttonBox);
    //browser.showClassPropertyTemplate(view->getClassName());
    //PropertyWidget::getInstance()->show
}

NewPropertyDialog::~NewPropertyDialog()
{
    delete ui;
}
