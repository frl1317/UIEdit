#ifndef UIXML_H
#define UIXML_H

#include "view.h"

class UIXML : public View
{
public:
    UIXML();
    ~UIXML();

    bool load(QXmlStreamReader &reader, QSize size);
    void skipUnknownElement(QXmlStreamReader &reader);

    View* findView(const QString &name);

    virtual QRectF boundingRect() const { return QRectF( 0, 0, size.width(), size.height());}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void applyProperty();
private:
};

#endif // UIXML_H
