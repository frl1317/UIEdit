#include "gamescene.h"
#include "qdebug.h"
#include <QGraphicsSceneMouseEvent>

#include <QFileDialog>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

#include <QMimeData>
#include <QMessageBox>
#include <QKeyEvent>
#include "mainwindow.h"
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include "view.h"
#include "uixml.h"
#include "qmenu.h"
#include <QAction>

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent),
    topView(NULL)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setBackgroundBrush(QBrush(QColor( 109, 109, 109)));

   // addItem(&screenSprite);

    setSceneSize(QSize(640, 960));

    reset();
}

GameScene::GameScene(const QSize &size, QObject *parent) :
    QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setBackgroundBrush(QBrush(QColor( 109, 109, 109)));

    //addItem(&screenSprite);

    setSceneSize(size);
}

GameScene::~GameScene()
{
    this->clear();
}

void GameScene::setSceneSize(const QSize &size)
{
    sceneSize = size;
    setSceneRect( -size.width(), -size.height(), size.width()*2, size.height()*2);

    if(size.width() == 320 && size.height() == 480){
        screenSprite.setPixmap(QPixmap(":/Resources/frame-iphone.png"));
        screenSprite.setOffset( -screenSprite.boundingRect().width() * 0.5f,
                                -screenSprite.boundingRect().height() * 0.5f);
        screenSprite.setPos(0,0);
        screenSprite.setRotation(0);
        screenSprite.setScale(1.0f);
    }
    else if(size.width() == 480 && size.height() == 320){
        screenSprite.setPixmap(QPixmap(":/Resources/frame-iphone.png"));
        screenSprite.setOffset( -screenSprite.boundingRect().width() * 0.5f,
                                -screenSprite.boundingRect().height() * 0.5f);
        screenSprite.setPos(0,0);
        screenSprite.setRotation(90);
        screenSprite.setScale(1.0f);
    }
    else if(size.width() == 640 && size.height() == 960){
        screenSprite.setPixmap(QPixmap(":/Resources/frame-iphone.png"));
        screenSprite.setOffset( -screenSprite.boundingRect().width() * 0.5f,
                                -screenSprite.boundingRect().height() * 0.5f);
        screenSprite.setPos(0,0);
        screenSprite.setRotation(0);
        screenSprite.setScale(2.0f);
    }
    else if(size.width() == 960 && size.height() == 640){
        screenSprite.setPixmap(QPixmap(":/Resources/frame-iphone.png"));
        screenSprite.setOffset( -screenSprite.boundingRect().width() * 0.5f,
                                -screenSprite.boundingRect().height() * 0.5f);
        screenSprite.setPos(0,0);
        screenSprite.setRotation(90);
        screenSprite.setScale(2.0f);
    }
    else if(size.width() == 768 && size.height() == 1024){
        screenSprite.setPixmap(QPixmap(":/Resources/frame-ipad.png"));
        screenSprite.setOffset( -screenSprite.boundingRect().width() * 0.5f,
                                -screenSprite.boundingRect().height() * 0.5f);
        screenSprite.setPos(0,0);
        screenSprite.setRotation(0);
        screenSprite.setScale(1.0f);
    }
    else if(size.width() == 1024 && size.height() == 768){
        screenSprite.setPixmap(QPixmap(":/Resources/frame-ipad.png"));
        screenSprite.setOffset( -screenSprite.boundingRect().width() * 0.5f,
                                -screenSprite.boundingRect().height() * 0.5f);
        screenSprite.setPos(0,0);
        screenSprite.setRotation(90);
        screenSprite.setScale(1.0f);
    }else{
        screenSprite.setVisible(false);
    }
    screenSprite.setZValue( ForegroundImage_Z);
}

void GameScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter,rect);
    painter->fillRect(-sceneSize.width()/2, -sceneSize.height()/2,sceneSize.width(),sceneSize.height(), QColor(255,255,255));
}
void GameScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawForeground(painter,rect);
    painter->setPen(QColor(0,255,0));
    painter->drawLine(rect.x(), 0, rect.width(), 0);
    painter->drawLine(0, rect.y(), 0, rect.height());
}

void GameScene::addItem(QGraphicsItem *item)
{
    QGraphicsScene::addItem(item);
    //signalAddItem(item);
}

void GameScene::groupLoad(QXmlStreamReader& reader)
{
    topView->propertyClear();
    topView->setText(0, reader.attributes().value("id").toString());
    topView->setText(1, "ui");
    topView->loadXML_BaseInfo(reader, QSize());

    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if(reader.name().toString() == "script"){
                View *view = View::NewView(reader.name().toString());
                topView->addSubView(view);
                view->loadXML_BaseInfo(reader, sceneSize);
                view->script_code = reader.readElementText();
            }else{
                View *view = View::NewView(reader.name().toString());
                topView->addSubView(view);
                view->loadXML(reader, getSceneSize(), topView);
            }
        }else{
            reader.readNext();
        }
    }
}

void GameScene::skipUnknownElement(QXmlStreamReader &reader)
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
             reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            return skipUnknownElement(reader);
        } else {
            reader.readNext();
        }
    }
}

bool GameScene::readXML()
{
    readXML(getPath());
}

bool GameScene::readXML(const QString& xmlFile)
{
    if(xmlFile.contains(".xml") == false){
        return false;
    }

    QXmlStreamReader reader;
    QFile file(xmlFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    setPath(xmlFile);
    reader.setDevice(&file);

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "ui") {
                groupLoad(reader);
            } else {
                reader.raiseError(tr("Not a valid book file"));
            }
        } else {
             reader.readNext();
        }
    }
    file.close();
    if (reader.hasError()) {
        QMessageBox::critical(NULL, tr("Error"),
                              tr("Failed to parse file %1").arg(xmlFile));
        return false;
    } else if (file.error() != QFile::NoError) {
        QMessageBox::critical(NULL, tr("Error"),
                              tr("Cannot read file %1").arg(xmlFile));
        return false;
    }

    return true;
}


bool GameScene::saveXML()
{
    saveXML(getPath());
}

bool GameScene::saveXML(const QString& xmlFile)
{
    QFile file(xmlFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Error: Cannot write file: " << qPrintable(file.errorString());
        return false;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    topView->saveXML(xmlWriter);
    xmlWriter.writeEndDocument();

    file.close();

    if (file.error()) {
        qDebug() << "Error: Cannot write file: "
                 << qPrintable(file.errorString());
        return false;
    }

    return true;
}

void GameScene::reset()
{
    clear();
    topView = NULL;
    topView = View::NewView("ui");
    topView->propertyAdd( "id", QFileInfo(path).fileName().remove(".xml"));
    topView->applyProperty();
    this->addItem(topView);
}

//void GameScene::keyPressEvent(QKeyEvent *event)
//{
//    QGraphicsScene::keyPressEvent(event);
//    QList<QGraphicsItem *>list = selectedItems();
//    if(list.count() > 0)
//    {
//        int x = 0, y = 0;
//        switch(event->key())
//        {
//        case Qt::Key_Control:
//            break;
//        case Qt::Key_Delete:
//        case Qt::Key_Backspace:
//            delete
//            for( int i = list.count() - 1; i >= 0; i--){
//                QGraphicsItem *item = (QGraphicsItem *)list.at(i);
//                this->removeItem(item);
//            }
//            break;
//        }
//        for( int i = 0; i < list.count(); i++){
//            View *view = (View *)list.at(i);
//            int viewX = 0, viewY = 0;
//            switch (view->getPosType()) {
//            case View::FromParent:
//                viewX = view->getPropertyByKey("offsetX").toFloat();
//                viewY = view->getPropertyByKey("offsetY").toFloat();
//                view->propertyUpdate("offsetX", QString::number(viewX + x));
//                view->propertyUpdate("offsetY", QString::number(viewY + y));
//                break;
//            case View::Absolute:
//                viewX = view->getPropertyByKey("x").toFloat();
//                viewY = view->getPropertyByKey("y").toFloat();
//                view->propertyUpdate("x", QString::number(viewX + x));
//                view->propertyUpdate("y", QString::number(viewY + y));
//                break;
//            default:
//                break;
//            }
//        }
//    }
//}

void GameScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    clearSelection();
}

void GameScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    clearSelection();
    View *view = (View *)itemAt(event->scenePos(), QTransform());
    if(view){
        view->QGraphicsItem::setSelected(true);
    }
}


void GameScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    View *view = (View *)itemAt(event->scenePos(), QTransform());
    if(view){
        clearSelection();
        View *newView = View::NewView(event->mimeData()->text());
        newView->propertyAdd("id", view->getID() + "_sub_"+newView->getClassName());
        newView->QGraphicsItem::setSelected(true);
        view->addSubView(newView);
    }else{
        View *newView = View::NewView(event->mimeData()->text());
        newView->QGraphicsItem::setSelected(true);
        topView->addSubView(newView);
    }
}
