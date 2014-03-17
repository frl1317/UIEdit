#include "windows.h"

Windows::Windows():
    View(),
    fillColor(0xA0000000)
{
}

void Windows::applyProperty()
{
    View::applyProperty();

    QString prop;
    prop = getPropertyByKey("fX");
    if(prop.isEmpty() == false)
    {
        fillRect.setX(prop.toInt());
    }

    prop = getPropertyByKey("fY");
    if(prop.isEmpty() == false)
    {
        fillRect.setY(prop.toInt());
    }

    prop = getPropertyByKey("fW");
    if(prop.isEmpty() == false)
    {
        fillRect.setWidth(prop.toInt());
    }

    prop = getPropertyByKey("fH");
    if(prop.isEmpty() == false)
    {
        fillRect.setHeight(prop.toInt());
    }

    prop = getPropertyByKey("fC");
    if(prop.isEmpty() == false)
    {
        fillColor = prop.toUInt(0, 16);
    }
}

void Windows::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    View::paint( painter, option, widget);

    painter->fillRect( fillRect, QColor( qRed(fillColor), qGreen(fillColor), qBlue(fillColor), qAlpha(fillColor)));

    painter->drawRect( 0, 0, size.width(), size.height());
}
