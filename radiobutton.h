#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H
#include "view.h"
class RadioButton : public View
{
public:
    RadioButton();
    virtual void applyProperty();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setImage(const QString& filename);

    QString onClick;
    QString background;
    int mode;

private:
    QPixmap pixmap;
};

#endif // RADIOBUTTON_H
