#include "uixml.h"
#include <QFile>
#include "view.h"

UIXML::UIXML(QSize size):
    size(size)
{
    //this->setText(0, "UIXML");
}

UIXML::~UIXML()
{

}

bool UIXML::load(QXmlStreamReader &reader)
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

    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "script") {
                skipUnknownElement(reader);
            } else {
                View *view = View::NewView(reader.name().toString());
                addSubView(view);
               // view->loadXML(reader, size, this);
            }
        }else{
            reader.readNext();
        }
    }

    return true;
}
void UIXML::addSubView(View* view)
{
    view->setParentItem(this);
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
