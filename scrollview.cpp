#include "scrollview.h"

ScrollView::ScrollView()
{
}


void ScrollView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    View::paint( painter, option, widget);

    painter->drawRect(this->boundingRect());
}
