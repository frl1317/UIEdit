#ifndef WINDOWS_H
#define WINDOWS_H
#include "view.h"

class Windows : public View
{
public:
    Windows();
    virtual int type() const{ return Type_Window;}
    virtual void applyProperty();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF fillRect;
    int fillColor;
};

#endif // WINDOWS_H
