#ifndef SCROLLVIEW_H
#define SCROLLVIEW_H
#include "view.h"
class ScrollView : public View
{
public:
    ScrollView();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // SCROLLVIEW_H
