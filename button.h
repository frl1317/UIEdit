#ifndef BUTTON_H
#define BUTTON_H
#include "view.h"
#include <QTextItem>

class Button : public View
{
public:
    Button();
    virtual void applyProperty();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setImage(const QString& filename);
    virtual int type() const{ return Type_Button;}

    const QString getOnClick(){ return onClick;}
    const QString getOnRelease(){ return onRelease;}
    const QString getOnHold(){ return onHold;}
    const QString getTextImageFile(){ return textImageFile;}
public slots:
private:
    QtProperty *_imageProperty;
    QPixmap pixmap;
    QPixmap textImage;
    QString textImageFile;
    QString text;
    int textColor;
    int textOffsetX;
    int textOffsetY;
    Qt::AlignmentFlag flag;
    QString onClick;
    QString onRelease;
    QString onHold;

};

#endif // BUTTON_H
