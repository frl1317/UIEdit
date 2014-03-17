#include "image.h"
#include "configfile.h"

Image::Image():
    View()
{
}

void Image::applyProperty()
{
    View::applyProperty();

    QString prop;
    prop = getPropertyByKey("src");
    if(prop.isEmpty() == false)
    {
        setImage(prop);
    }
}

void Image::setImage(const QString &filename)
{
    QString imageName = getConfigFile()->getFullPath(filename);
    bool load = pixmap.load(imageName);
    if(!load)
        LOG(imageName + "加载失败");
    this->filename = filename;
}

void Image::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    View::paint( painter, option, widget);
    painter->drawPixmap( 0, 0, pixmap);
}
