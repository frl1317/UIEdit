#ifndef RICHTEXT_H
#define RICHTEXT_H
#include "view.h"
#include "label.h"
class RichText : public View
{
public:
    RichText();
    virtual void loadXML_BaseInfo(QXmlStreamReader &reader, const QSize& contextSize);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QString text;
};

#endif // RICHTEXT_H
