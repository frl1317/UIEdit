#include "radiobutton.h"

RadioButton::RadioButton()
{
}

void RadioButton::applyProperty()
{
    View::applyProperty();

    QString prop;
    prop = getPropertyByKey("onClick");
    if(prop.isEmpty() == false)
    {
        onClick = prop;
    }


    prop = getPropertyByKey("background");
    if(prop.isEmpty() == false)
    {
        background = prop;
        setImage(background);
    }

    prop = getPropertyByKey("mode");
    if(prop.isEmpty() == false)
    {
        mode = prop.toInt();
    }
}
void RadioButton::setImage(const QString& filename)
{
    background = filename;
    QString imageName = getConfigFile()->getFullPath(filename);
    imageName.replace(QString("%s"), QString(""));

    bool load = pixmap.load(imageName);
    if(!load)
        LOG(imageName + "加载失败");
}

void RadioButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    View::paint( painter, option, widget);
    painter->drawPixmap( (size.width() - pixmap.width())/2, (size.height() - pixmap.height())/2, pixmap);
}
