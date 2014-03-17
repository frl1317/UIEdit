#include "button.h"
#include <QDebug>
Button::Button():
    View(),
    textOffsetX(0),
    textOffsetY(0)
{
   flag = Qt::AlignCenter;
}

void Button::applyProperty()
{
    View::applyProperty();

    QString prop;

    prop = getPropertyByKey("backImage");
    if(prop.isEmpty() == false)
    {
        QString filename = prop;
        filename.replace(QString("%s"), QString(""));
        setImage(filename);
    }

    prop = getPropertyByKey("textImage");
    if(prop.isEmpty() == false)
    {
        textImageFile = prop;
        QString imageName = getConfigFile()->getFullPath(textImageFile);
        bool load = textImage.load(imageName);
        if(!load)
            LOG(textImageFile + "加载失败");
    }

    prop = getPropertyByKey("text");
    if(prop.isEmpty() == false)
    {
        text = prop;
    }

    prop = getPropertyByKey("textColor");
    if(prop!=NULL)
    {
        textColor=prop.toInt();
    }
    prop = getPropertyByKey("textOffsetX");
    if(prop!=NULL)
    {
        textOffsetX=prop.toInt();
    }
    prop = getPropertyByKey("textOffsetY");
    if(prop!=NULL)
    {
        textOffsetY=prop.toInt();
    }

    prop = getPropertyByKey("onClick");
    if(prop!=NULL)
    {
        onClick=prop;
    }
    prop = getPropertyByKey("onHold");
    if(prop!=NULL)
    {
        onHold=prop;
    }
    prop = getPropertyByKey("onRelease");
    if(prop!=NULL)
    {
        onRelease=prop;
    }
}

void Button::setImage(const QString& filename)
{
    QString imageName = getConfigFile()->getFullPath(filename);
    bool load = pixmap.load(imageName);
    if(!load)
        LOG(imageName + "加载失败");
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    View::paint( painter, option, widget);
    painter->drawPixmap( (size.width() - pixmap.width())/2, (size.height() - pixmap.height())/2, pixmap);
    painter->drawPixmap( (size.width() - textImage.width())/2, (size.height() - textImage.height())/2, textImage);
    painter->drawText( textOffsetX, textOffsetY, size.width(), size.height(), flag, text);
}

