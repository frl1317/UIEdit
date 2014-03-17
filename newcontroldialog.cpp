#include "newcontroldialog.h"
#include "ui_newcontroldialog.h"
#include "view.h"
#include "gamescene.h"

NewControlDialog::NewControlDialog(GameScene *scene) :
    ui(new Ui::NewControlDialog)
{
    ui->setupUi(this);

    this->scene = scene;

//    QSetIterator<QtProperty *> itr( browser.group->properties());
//    QStringList list;
//    while(itr.hasNext()){
//        QtProperty *pp = itr.next();
//        list << pp->propertyName();
//    }
//    list.sort();
//    for(int i = 0;i<list.count();i++){
//        ui->comboBox->addItem(list.at(i));
//    }

//    if(this->scene->selectedItems().count() > 0)
//    {
//        View* parent = (View*)this->scene->selectedItems().at(0);
//        if(parent){
//            parentname = parent->getID();
//        }
//    }else{
//        parentname = this->scene->topView->getID();
//    }

    //connect( browser.variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(propertyChange(QtProperty*,QVariant)));
}

NewControlDialog::~NewControlDialog()
{
    delete ui;
}

void NewControlDialog::propertyChange(QtProperty* property,QVariant variant)
{
//    if(browser.currentView){
//        browser.currentView->propertyUpdate(property->propertyName(), variant.toString());
//    }
}
View* NewControlDialog::buildView()
{
//    View *view = View::NewView(ui->comboBox->currentText());
//    view->propertyAdd("id", ui->lineEdit->text());
//    view->setConSize(scene->getSceneSize());
//    browser.initInfoForView(view);
//    return view;
}
