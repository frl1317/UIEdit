#include "uixml.h"
#include <QFile>
#include "view.h"

UIXML::UIXML()
{
    //this->setText(0, "UIXML");
    setClassName("ui");

}

UIXML::~UIXML()
{

}

bool UIXML::load(QXmlStreamReader &reader, QSize size)
{
    if(reader.attributes().value("id").isEmpty() == false)
    {
        id = reader.attributes().value("id").toString();
    }
    if(reader.attributes().value("z").isEmpty() == false)
    {
       // setZValue(reader.attributes().value("z").toInt());
    }
    if(reader.attributes().value("hideRelease").isEmpty() == false)
    {
    }
    setText(0, id);
    setText(1, "ui");

    loadXML_BaseInfo(reader, size);
    applyProperty();

    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if(reader.name().toString() == "script"){
                View *view = View::NewView(reader.name().toString());
                this->addSubView(view);
                view->loadXML_BaseInfo(reader, size);
                view->script_code = reader.readElementText();
            }else{
                View *view = View::NewView(reader.name().toString());
                this->addSubView(view);
                view->loadXML(reader, size, this);
            }
        }else{
            reader.readNext();
        }
    }
    return true;
}

View* UIXML::findView(const QString &name)
{
    if(name.isEmpty())
        return NULL;
    QListIterator<QGraphicsItem *> itr(childItems());
    while(itr.hasNext()){
        View *view = (View*)itr.next();
        if(view->getID() == name)
        {
            return view;
        }
    }
    return NULL;
}
void UIXML::skipUnknownElement(QXmlStreamReader &reader)
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
             reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            return skipUnknownElement(reader);
        } else {
            reader.readNext();
        }
    }
}

void UIXML::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
{
}

void UIXML::applyProperty()
{
    View::applyProperty();
    setPos(-size.width()/2, -size.height()/2);
}
