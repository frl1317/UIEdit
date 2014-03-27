#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "view.h"
#include <QPixmap>

class CheckBox : public View
{
public:
    CheckBox();
    virtual void applyProperty();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    bool checked;
    QPixmap bgPixmap;
    QPixmap onPixmap;
};

#endif // CHECKBOX_H
