#include "propgrid.h"

PropGrid::PropGrid()
{
}

void PropGrid::applyProperty()
{
    RadioButton::applyProperty();

    QString prop;
    prop = getPropertyByKey("numalign");
    if(prop.isEmpty() == false)
    {
    }

    prop = getPropertyByKey("wall");
    if(prop.isEmpty() == false)
    {
    }

    prop = getPropertyByKey("fontName");
    if(prop.isEmpty() == false)
    {
    }
}
void PropGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    RadioButton::paint( painter, option, widget);
}
