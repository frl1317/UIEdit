#ifndef GROUP_H
#define GROUP_H

#include "view.h"

class Group : public View
{
public:
    Group();
    virtual int type() const{ return Type_Group;}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void applyProperty();

private:
    int fillColor;
    int lineColor;
    bool isRoundRect;
};

#endif // GROUP_H
