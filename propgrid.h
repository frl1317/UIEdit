#ifndef PROPGRID_H
#define PROPGRID_H

#include "RadioButton.h"

class PropGrid : public RadioButton
{
public:
    PropGrid();
    virtual void applyProperty();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // PROPGRID_H
