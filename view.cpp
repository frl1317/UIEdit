#include "view.h"
#include "gamescene.h"
#include "image.h"
#include "configfile.h"
#include "group.h"
#include "label.h"
#include "button.h"
#include "windows.h"
#include "radiobutton.h"
#include "PropGrid.h"
#include "richtext.h"
#include "scrollview.h"
#include <QStyleOptionGraphicsItem>
#include "uixml.h"

View::View(QObject *parent):
    QGraphicsItem(),
    color(-1),
    className(QString())
{
    QGraphicsItem::setFlags(ItemIsSelectable);//|ItemIsMovable
}
View::~View()
{
    while(propertys.count()>0)
    {
        MyProperty *p = propertys.at(0);
        propertys.removeAt(0);
        delete p;
    }
}

View* View::NewView(const QString& tag)
{
    QString lowerTag = tag.toLower();
    View *view = NULL;
    if( lowerTag == "view"){
        view = new View();
    }
    else if( lowerTag == "group"){
        view = new Group();
    }
    else if( lowerTag == "image"){
        view = new Image();
    }
    else if( lowerTag == "label"){
        view = new Label();
    }
    else if( lowerTag == "button"){
        view = new Button();
    }
    else if( lowerTag == "window"){
        view = new Windows();
    }
    else if( lowerTag == "radiobutton"){
        view = new RadioButton();
    }
    else if( lowerTag == "propgrid"){
        view = new PropGrid();
    }
    else if( lowerTag == "richtext"){
        view = new RichText();
    }else if( lowerTag == "scrollview"){
        view = new ScrollView();
    }else{
        view = new View();
    }

    view->setClassName(lowerTag);
    view->setText( 0, "NULL");
    view->setText( 1, lowerTag);
    view->QTreeWidgetItem::setFlags(view->QTreeWidgetItem::flags() | Qt::ItemIsUserCheckable);
    view->QTreeWidgetItem::setCheckState(0, Qt::Checked);

    return view;
}

void View::loadXML(QXmlStreamReader &reader, const QSize& contextSize, View* xml)
{
    this->group = xml;
    loadXML_BaseInfo(reader, contextSize);
    applyProperty();
    loadXML_SubNode(reader);
}

void View::saveXML(QXmlStreamWriter &writer)
{
    writer.writeStartElement(getClassName());

    for( int i = 0; i < propertys.count(); ++i)
    {
        MyProperty *p = propertys.at(i);
        QString value = PropertyWidget::getInstance()->getPropertyValueByName(p->name);
        if(value != p->value)
            writer.writeAttribute(p->name, p->value);
    }
    writer.writeCharacters(script_code);
    saveXML_SubNode(writer);
    writer.writeEndElement();
}

void View::saveXML_SubNode(QXmlStreamWriter &writer)
{
    int count = childCount();
    for( int i = 0; i < count; i++){
        View *view = (View*)child(i);
        view->saveXML(writer);
    }
}

bool View::propertyContains(const QString &key)
{
    for( int i = 0; i < propertys.count(); ++i)
    {
        MyProperty *p = propertys.at(i);
        if(p->name == key){
            return true;
        }
    }
    return false;
}

void View::propertyAdd(const QString &key, const QString &value)
{
    if(propertyContains(key) == false){
        MyProperty *p = new MyProperty();
        p->name = key;
        p->value = value;
        propertys.append(p);
    }
}

void View::propertyUpdate(const QString &key, const QString &value)
{
    if(propertyContains(key)){
        for( int i = 0; i < propertys.count(); ++i)
        {
            MyProperty *p = propertys.at(i);
            if(p->name == key){
                p->value = value;
                break;
            }
        }
        applyProperty();
    }
}
void View::propertyRemove(const QString &key)
{
    for( int i = 0; i < propertys.count(); ++i)
    {
        MyProperty *p = propertys.at(i);
        if(p->name == key){
            propertys.removeAll(p);
            applyProperty();
            delete p;
            break;
        }
    }
}

void View::propertyClear()
{
    while(propertys.count()>0)
    {
        MyProperty *p = propertys.at(0);
        propertys.removeAt(0);
        delete p;
    }
}

QString View::getPropertyByKey(const QString &key)
{
    for( int i = 0; i < propertys.count(); ++i)
    {
        MyProperty *p = propertys.at(i);
        if(p->name == key){
            return p->value;
        }
    }
    return NULL;
}
void View::applyProperty()
{
    float x = 0, y = 0;
    float w = conSize.width();
    float h = conSize.height();

    QString prop;
    prop = getPropertyByKey("id");
    if(prop.isEmpty() == false)
    {
        id = prop;
    }

    prop = getPropertyByKey("z");
    if(prop.isEmpty() == false)
    {
        setZValue(prop.toInt());
    }

    prop = getPropertyByKey("touch");
    if(prop.isEmpty() == false)
    {
    }

    prop = getPropertyByKey("color");
    if(prop.isEmpty() == false)
    {
        color = prop.toUInt(0, 16);
    }

    prop = getPropertyByKey("width");
    if(prop.isEmpty() == false)
    {
        if(prop.contains('%'))
        {
            int pre=prop.remove('%').toInt();
            w=w*pre/100;
        }else{
            w=prop.toInt();
        }
    }

    prop = getPropertyByKey("height");
    if(prop.isEmpty() == false)
    {
        if(prop.contains('%'))
        {
            int pre=prop.remove('%').toInt();
            h=h*pre/100;
        }else{
            h=prop.toInt();
        }
    }

    prop = getPropertyByKey("angle");
    if(prop.isEmpty() == false)
    {
    }

    prop = getPropertyByKey("adaptive");
    if(prop.isEmpty() == false)
    {
    }

    prop = getPropertyByKey("grey");
    if(prop.isEmpty() == false)
    {
    }

    QString point = getPropertyByKey("point").toLower();
    QString relativePoint = getPropertyByKey("relativePoint").toLower();
    QString relativeTo = getPropertyByKey("relativeTo");
    QString offsetX = getPropertyByKey("offsetX");
    QString offsetY = getPropertyByKey("offsetY");
    if( point.isEmpty() == false && relativePoint.isEmpty() == false){
        float tX = 0, tY = 0;
        View *superView = (View*)this->parentItem();
        View *toView = NULL;
        float tWidth = conSize.width(),tHeight= conSize.height();
        this->relativeTo = relativeTo;
        if(relativeTo.isEmpty()){
            toView = superView;
        }else if(superView != NULL)
        {
            toView = superView->find(relativeTo);
        }else{
            toView = group->find(relativeTo);
        }
        if(toView != NULL){
            tX = toView->x();
            tY = toView->y();
            tWidth = toView->getSize().width();
            tHeight = toView->getSize().height();
        }
        if(relativePoint == "topleft")
        {

        }
        else if(relativePoint == "topright")
        {
            tX+=tWidth;
        }
        else if(relativePoint == "top")
        {
            tX+=tWidth/2;
        }
        else if(relativePoint == "left")
        {
            tY+=tHeight/2;
        }
        else if(relativePoint == "center")
        {
            tX+=tWidth/2;
            tY+=tHeight/2;
        }
        else if(relativePoint == "right")
        {
            tX+=tWidth;
            tY+=tHeight/2;
        }
        else if(relativePoint == "bottomleft")
        {
            tY+=tHeight;
        }
        else if(relativePoint == "bottomright")
        {
            tX+=tWidth;
            tY+=tHeight;
        }
        else if(relativePoint == "bottom")
        {
            tX+=tWidth/2;
            tY+=tHeight;
        }

        if( point == "top_left")
        {

        }else if(point == "top")
        {
            tX-=w/2;
        }
        else if(point == "topright")
        {
            tX-=w;
        }
        else if(point == "left")
        {
            tY-=h/2;
        }else if(point == "center")
        {
            tX-=w/2;
            tY-=h/2;
        }else if(point == "right")
        {
            tX-=w;
            tY-=h/2;
        }
        else if(point == "bottomleft")
        {
            tY-=h;
        }
        else if(point == "bottomright")
        {
            tX-=w;
            tY-=h;
        }
        else if(point == "bottom")
        {
            tX-=w/2;
            tY-=h;
        }

        if(offsetX.isEmpty() == false)
        {
            offsetPoint.setX(offsetX.toFloat());
            tX+=offsetPoint.x();
        }
        if(offsetY.isEmpty() == false)
        {
            offsetPoint.setY(offsetY.toFloat());
            tY+=offsetPoint.y();
        }
        x=tX;
        y=tY;
        if(toView!=NULL&&toView==superView)
        {
            x-=toView->x();
            y-=toView->y();
        }
        posType = FromParent;
    }else{

        prop = getPropertyByKey("x");
        if(prop.isEmpty() == false)
        {
            QString xStr = prop;
            if(prop.contains('%'))
            {
                int pre=xStr.remove('%').toInt();
                x=w*pre/100;
            }else{
                x=prop.toInt();
            }
        }

        prop = getPropertyByKey("y");
        if(prop.isEmpty() == false)
        {
            QString yStr = prop;
            if(prop.contains('%'))
            {
                int pre=yStr.remove('%').toInt();
                y=h*pre/100;
            }else{
                y=prop.toInt();
            }
        }
        posType = Absolute;
    }

    prop = getPropertyByKey("hide");
    if(prop.isEmpty() == false)
    {
        if( prop == "true"){
            setVisible(false);
        }else{
            setVisible(true);
        }
    }

    prop = getPropertyByKey("needClip");
    if(prop.isEmpty() == false)
    {
    }

    prop = getPropertyByKey("onUpdate");
    if(prop.isEmpty() == false)
    {
    }

    prop = getPropertyByKey("checkMoveEvent");
    if(prop.isEmpty() == false)
    {
    }

    setPos(x, y);
    setSize(w, h);

    setText(0,getID());
    setText(1,getClassName());
    prepareGeometryChange();
}

void View::loadXML_BaseInfo(QXmlStreamReader &reader, const QSize& contextSize)
{
    conSize = size = contextSize;
    QVectorIterator<QXmlStreamAttribute> itr = reader.attributes();
    while(itr.hasNext())
    {
        QXmlStreamAttribute attrib =  itr.next();
        propertyAdd(attrib.name().toString(), attrib.value().toString());
    }
}

void View::loadXML_SubNode(QXmlStreamReader &reader)
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
             reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            View *view = View::NewView(reader.name().toString());
            addSubView(view);
            view->loadXML(reader, size, group);
        }else {
            reader.readNext();
        }
    }
}

void View::addSubViewByIndex(int index, View *view)
{
    view->group = this->group;
    view->conSize = conSize;
    this->insertChild(index, view);
    childItems().insert( index, view);
    view->setParentItem(this);
    view->applyProperty();
}

void View::addSubView(View *view)
{
    view->group = this->group;
    view->conSize = size;
    this->addChild(view);
    childItems().append(view);
    view->setParentItem(this);
    view->applyProperty();
}

void View::removeSubView(View *view)
{
    this->removeChild(view);
    view->setParentItem(NULL);
    this->scene()->removeItem(view);
}

void View::setSize(QSize size)
{
    this->size = size;
}

View* View::find(const QString &name)
{
    if(name.isEmpty())
        return NULL;

    if( name == id)
        return this;

    for( int i = 0; i < childItems().count(); i++)
    {
        View *view = (View*)childItems().at(i);
        if(view->id == name)
        {
            return view;
        }
    }
    return NULL;
}

QRectF View::boundingRect() const
{
    return QRectF( 0, 0, size.width(), size.height());
}

void View::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black,1,Qt::SolidLine));
    painter->drawRect( 0, 0, size.width(),size.height());

    if(option->state & QStyle::State_Selected){
        painter->setPen(QPen(Qt::red, 2,Qt::DotLine));
        painter->drawRect( 0, 0, size.width(),size.height());
    }

   // painter->drawRect( 0, 0, size.width(), size.height());
}

View *View::copyView()
{
    View *view = View::NewView(this->getClassName());
    for( int i = 0; i < getPropertys().count(); ++i)
    {
       MyProperty *p = getPropertys().at(i);
       view->propertyAdd( p->name, p->value);
    }

    view->group = group;

    for( int i = 0; i < childItems().count(); ++i)
    {
        View * sub = (View*)childItems().at(i);
        view->addSubView(sub->copyView());
    }
    view->applyProperty();
    return view;
}

QVariant View::itemChange(GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange( change, value);
}
