#include "operation.h"
#include "uixml.h"

OperationAddItem::OperationAddItem(View *view)
{
//    LOG("添加了OperationAddItem");
//    this->view = view->copyView();
}
OperationAddItem::~OperationAddItem()
{
//    LOG("释放了OperationAddItem");
//    delete view;
}
void OperationAddItem::undo()
{
//    if(view == (View*)view->getGroupView()){
//    }else{
//        View *parenView = (View *)view->parentItem();
//        parenView->removeSubView(view);
//        delete view;
//        view = NULL;
//    }
}


OperationDeleteItem::OperationDeleteItem(View *view)
{
//    LOG("添加了OperationDeleteItem");
//    className = view->getClassName();
//    for(int i = 0; i < view->propertys.count();i++)
//    {
//        MyProperty *p = view->propertys.at(i);
//        MyProperty *newP = new MyProperty();
//        newP->name = p->name;
//        newP->type = p->type;
//        newP->value = p->value;
//        propertys.append(newP);
//    }

}

OperationDeleteItem::~OperationDeleteItem()
{
//    LOG("释放了OperationDeleteItem");
//    while(propertys.count() > 0)
//    {
//        MyProperty *p = propertys.takeAt(0);
//        delete p;
//    }
//    propertys.clear();
}

void OperationDeleteItem::undo()
{
//    LOG("撤销");
//    View *view = View::NewView(className);
//    for(int i = 0; i < propertys.count();i++)
//    {
//        MyProperty *p = propertys.at(i);
//        view->propertyAdd(p->name, p->value);
//    }
//    view->applyProperty();
}
