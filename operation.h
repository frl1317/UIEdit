#ifndef OPERATION_H
#define OPERATION_H
#include "configfile.h"
#include "view.h"
#include "gamescene.h"

class Operation
{
public:
    virtual void undo() = 0;
    virtual void destroy()=0;
};

class OperationAddItem : public Operation
{
public:
    OperationAddItem(View *view);
    ~OperationAddItem();
    virtual void undo();
    virtual void destroy(){delete this;}
private:
    View *view;
};
class OperationDeleteItem : public Operation
{
public:
    OperationDeleteItem(View *view);
    ~OperationDeleteItem();
    virtual void undo();
    virtual void destroy(){delete this;}
private:
    QString className;
    QList<QVariant*> propertys;
};
#endif // OPERATION_H
