#include "group.h"

Group::Group():
    View(),
    fillColor(0xC0130E06),
    lineColor(0xFF9F9B93),
    isRoundRect(true)
{
}

void Group::applyProperty()
{
    View::applyProperty();

    QString prop;
    prop = getPropertyByKey("fillColor");
    if(prop.isEmpty() == false)
    {
        fillColor = prop.toUInt(0, 16);
    }

    prop = getPropertyByKey("lineColor");
    if(prop.isEmpty() == false)
    {
        lineColor = prop.toUInt(0, 16);
    }
}

void Group::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    View::paint( painter, option, widget);
    if(isRoundRect){
        painter->fillRect( 0, 0, size.width(), size.height(), QColor( qRed(fillColor), qGreen(fillColor), qBlue(fillColor), qAlpha(fillColor)));
        painter->setPen(QColor(qRed(lineColor), qGreen(lineColor), qBlue(lineColor), qAlpha(lineColor)));
        painter->drawRect(0, 0, size.width(), size.height());
    }else{
        painter->fillRect( 0, 0, size.width(), size.height(), QColor( qRed(fillColor), qGreen(fillColor), qBlue(fillColor), qAlpha(fillColor)));
        painter->setPen(QColor(qRed(lineColor), qGreen(lineColor), qBlue(lineColor), qAlpha(lineColor)));
        painter->drawRect(0, 0, size.width(), size.height());
    }

}
