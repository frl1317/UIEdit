#ifndef IMAGE_H
#define IMAGE_H

#include "view.h"

class Image : public View
{
public:
    Image();
    virtual int type() const{ return Type_Image;}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void applyProperty();

    void setImage(const QString &filename);
    QString getImage() { return filename;}
private:
    QPixmap pixmap;
    QString filename;

};

#endif // IMAGE_H
