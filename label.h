#ifndef LABEL_H
#define LABEL_H

#include "view.h"
#include <QStaticText>

class Label : public View
{
public:
    Label();

    QString getText() const{return text;}
    void setText(const QString &text);
    virtual int type() const{ return Type_Label;}

    virtual void applyProperty();
    virtual void loadXML_SubNode(QXmlStreamReader &reader);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QFont font;
    QString text;
    int flag;
};

#endif // LABEL_H
