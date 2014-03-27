#include "newcontroldialog.h"
#include "ui_newcontroldialog.h"
#include "view.h"
#include "gamescene.h"
#include "uixml.h"

NewControlDialog::NewControlDialog(GameScene *scene) :
    ui(new Ui::NewControlDialog)
{
    ui->setupUi(this);

    this->scene = scene;


    QSetIterator<QtProperty *> itr( PropertyWidget::getInstance()->group->properties());
    QStringList list;
    while(itr.hasNext()){
        QtProperty *pp = itr.next();
        list << pp->propertyName();
    }
    list.sort();
    for(int i = 0;i<list.count();i++){
        ui->comboBox->addItem(list.at(i));
    }

    if(this->scene->selectedItems().count() > 0)
    {
        View* parent = (View*)this->scene->selectedItems().at(0);
        if(parent){
            parentname = parent->getID();
        }
    }else{
        parentname = this->scene->topView->getID();
    }

    connect( PropertyWidget::getInstance()->variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(propertyChange(QtProperty*,QVariant)));
}

NewControlDialog::~NewControlDialog()
{
    delete ui;
}

void NewControlDialog::propertyChange(QtProperty* property,QVariant variant)
{
    if(PropertyWidget::getInstance()->currentView){
        PropertyWidget::getInstance()->currentView->propertyUpdate(property->propertyName(), variant.toString());
    }
}
View* NewControlDialog::buildView()
{
    View *view = View::NewView(ui->comboBox->currentText());
    view->propertyAdd("id", ui->lineEdit->text());
    view->propertyUpdate("id", ui->lineEdit->text());
    view->setConSize(scene->getSize());
    return view;
}
