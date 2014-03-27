#include "checkbox.h"

CheckBox::CheckBox():
    View()
{
}
void CheckBox::applyProperty()
{
    View::applyProperty();

    QString prop;
    prop = getPropertyByKey("normalImage");
    if(prop.isEmpty() == false)
    {
        QString imageName = getConfigFile()->getFullPath(prop);
        bool load = bgPixmap.load(imageName);
        if(!load)
            LOG(imageName + "加载失败");
    }

    prop = getPropertyByKey("onChecked");
    if(prop.isEmpty() == false)
    {
    }

    prop = getPropertyByKey("checkedImage");
    if(prop.isEmpty() == false)
    {
        QString imageName = getConfigFile()->getFullPath(prop);
        bool load = onPixmap.load(imageName);
        if(!load)
            LOG(imageName + "加载失败");
    }

    prop = getPropertyByKey("checked");
    if(prop.isEmpty() == false)
    {
        if(prop == "false")
            checked = false;
        else if(prop == "true")
            checked = true;
    }
    prepareGeometryChange();
}

void CheckBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    View::paint( painter, option, widget);
    painter->drawPixmap( (size.width() - bgPixmap.width())/2, (size.height() - bgPixmap.height())/2, bgPixmap);
    if(checked)
    {
        painter->drawPixmap( (size.width() - onPixmap.width())/2, (size.height() - onPixmap.height())/2, onPixmap);
    }
}
