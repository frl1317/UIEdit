#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QXmlStreamReader>
#include "view.h"
#include <QTreeWidget>

#define BackgroundImage_Z -100000000
#define ForegroundImage_Z 100000000

class MainWindow;

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = 0);
    explicit GameScene(const QSize &size, QObject *parent = 0);
    ~GameScene();

    bool readXML();
    bool readXML(const QString& xmlFile);
    bool saveXML();
    bool saveXML(const QString& xmlFile);

    void skipUnknownElement(QXmlStreamReader &reader);

    void setSize(const QSize &size);
    const QSize getSize() {return size;}

    const QString& getSceneName(){ return path;}
    void setSceneName(const QString &name){path = name;}
    const QString& getPath(){ return path;}
    void setPath(const QString &path){this->path = path;}

    const QString& getResourcePath(){ return resourcePath;}
    void setResourcePath(const QString &path){this->resourcePath = path;}

    void addItem(QGraphicsItem *item);

    void creatUI(const QString name = 0);

   // void reset();

    UIXML *topView;

    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);
public slots:

private:
    QString resourcePath;
    QString path;
    QSize size;

    QImage backgroundImage;
    QImage foregroundImage;

    QGraphicsPixmapItem screenSprite;
protected:

    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);
};

#endif // GAMESCENE_H
