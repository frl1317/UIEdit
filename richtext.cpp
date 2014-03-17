#include "richtext.h"

RichText::RichText()
{
    text = "RichText";
}


void RichText::loadXML_BaseInfo(QXmlStreamReader &reader, const QSize& contextSize)
{
    View::loadXML_BaseInfo(reader, contextSize);
}
void RichText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    View::paint( painter, option, widget);
    painter->drawRect( 0, 0, size.width(), size.height());
    painter->drawText( 0, 0, size.width(), size.height(), 0, text);
}
