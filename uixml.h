#ifndef UIXML_H
#define UIXML_H

#include <Qt>
#include <QXmlStreamReader>
#include <QGraphicsItem>
#include <QTreeWidgetItem>

class View;

class UIXML : public QGraphicsItem
{
public:
    UIXML(QSize size);
    ~UIXML();


    bool load(QXmlStreamReader &reader);
    void skipUnknownElement(QXmlStreamReader &reader);

    View* findView(const QString &name);
    void addSubView(View* view);

    virtual QRectF boundingRect() const { return QRectF( 0, 0, size.width(), size.height());}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QSize size;
private:
    QString id;
};

#endif // UIXML_H
