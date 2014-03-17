#include "label.h"
#include <qDebug>
#include <QXmlStreamAttributes>

Label::Label():
    View()
{
    flag = Qt::AlignCenter;

    font.setPixelSize(20);
}

void Label::applyProperty()
{
    setText("");

    View::applyProperty();

    QString prop;

    prop = getPropertyByKey("align");
    if(prop.isEmpty() == false)
    {
        if(prop == "right")
            flag = Qt::AlignRight|Qt::AlignVCenter;
        else if(prop == "left")
            flag = Qt::AlignLeft|Qt::AlignVCenter;
        else if(prop == "center")
            flag = Qt::AlignCenter;
    }

    prop = getPropertyByKey("text");
    if(prop.isEmpty() == false)
    {
        setText(prop);
    }

    prop = getPropertyByKey("fontSize");
    if(prop.isEmpty() == false)
    {
        font.setPixelSize(prop.toInt());
    }

}

void Label::loadXML_SubNode(QXmlStreamReader &reader)
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
             reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            View *view = View::NewView(reader.name().toString());
            view->setParentItem(this);
            view->loadXML(reader, size, group);
        }
        else if(reader.isCharacters()){
            text = reader.text().toString();
            propertyAdd("text", text);
            reader.readNext();
        }
        else {
            reader.readNext();
        }
    }
}
void Label::setText(const QString &text)
{
    this->text = text;
}

void Label::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    View::paint( painter, option, widget);
    painter->setFont(font);
    painter->setPen(QColor(color));
    painter->drawText( 0, 0, size.width(), size.height(), flag, text);
}
