#ifndef VIEW_H
#define VIEW_H

#include "qgraphicsitem.h"
#include <QXmlStreamReader>
#include "qpainter.h"
#include "configfile.h"
#include "propertywidget.h"
#include <QDebug>
#include <QHash>
#include <QTreeWidgetItem>

class UIXML;

typedef struct _MyProperty
{
    QString name;
    QString type;
    QString value;
}MyProperty;

class View : public QGraphicsItem, public QObject, public QTreeWidgetItem
{
public:

    static View *NewView(const QString& tag);
    explicit View(QObject *parent = 0);
    ~View();

    void saveXML(QXmlStreamWriter &writer);
    void saveXML_SubNode(QXmlStreamWriter &writer);
    void loadXML(QXmlStreamReader &reader, const QSize& contextSize, UIXML* xml);
    virtual void loadXML_BaseInfo(QXmlStreamReader &reader, const QSize& contextSize);
    virtual void loadXML_SubNode(QXmlStreamReader &reader);
    virtual void applyProperty();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool propertyContains(const QString &key);
    void propertyAdd(const QString &key, const QString &value);
    void propertyUpdate(const QString &key, const QString &value);
    void propertyRemove(const QString &key);
    void propertyClear();
    QString getPropertyByKey(const QString &key);
    QList<MyProperty*> getPropertys() const { return propertys;}

    View* find(const QString &name);
    QSize getSize(){ return size;}
    void setSize(QSize size);
    void setSize(float widget, float height){
        size.setWidth(widget);
        size.setHeight(height);
    }
    enum {
        Type_View = 65536,
        Type_Image,
        Type_Button,
        Type_Label,
        Type_Group,
        Type_Window,
        Type_Max
    };
    virtual int type() const{ return Type_View;}
    QString getID() const {return id;}

    typedef enum _PosType{
        FromParent,
        Absolute
    }PosType;

    PosType getPosType() { return this->posType;}

    typedef enum _AnchorPoint{
        Anchor_TopLeft,
        Anchor_TopRight,
        Anchor_Top,
        Anchor_Left,
        Anchor_Center,
        Anchor_Right,
        Anchor_BottomLeft,
        Anchor_BottomRight,
        Anchor_PointBottom
    }AnchorPoint;

    void setClassName(const QString &className) { this->className = className;}
    const QString getClassName() { return className;}

    const QPointF getPoint() { return point;}
    AnchorPoint getRelativePoint() const { return relativePoint;}
    const QString getRelativeTo() { return relativeTo;}
    const QPointF getOffsetPoint() { return offsetPoint;}

    void setConSize(const QSize size) {  conSize = size;}
    void addSubView(View *view);
    void addSubViewByIndex(int index, View *view);
    void removeSubView(View *view);

    View *copyView();

    UIXML *getGroupView() { return group;}
    void setGroupView(UIXML *view) { group = view;}


    virtual void refresh();

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QString script_code;
public slots:
protected:
    UIXML *group;
    QString className;
    QSize size;
    QString id;
    QPointF point;
    QPointF offsetPoint;
    AnchorPoint relativePoint;
    QString relativeTo;
    PosType posType;

    int color;
    QSize conSize;
public:
    QList<MyProperty*> propertys;
};

#endif // VIEW_H
