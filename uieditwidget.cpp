#include "uieditwidget.h"
#include "ui_uieditwidget.h"
#include <QMenu>
#include <QKeyEvent>
#include "newcontroldialog.h"
#include <QFileDialog>
#include "newpropertydialog.h"
#include "uixml.h"

#define MaxScreenCount 8

static const int screenSize[MaxScreenCount][2] =
{
    {480,320},
    {960,640},
    {1024,768},
    {2048,1536},
    {320,480},
    {640,960},
    {768,1024},
    {1536,2048},
};

static View *copyView = NULL;

UIEditWidget::UIEditWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::UIEditWidget),
      modify(false),
      outlinewidget(NULL),
      propertywidget(NULL)
{
    ui->setupUi(this);

    scene = new GameScene(this);

    //初始化视图缩放条
    ui->viewHSlider->setValue(100);
    ui->viewHSlider->setMaximum(500);
    ui->viewHSlider->setMinimum(1);

    ui->doubleSpinBox->setValue(1.0f);
    ui->doubleSpinBox->setMaximum(5.0f);
    ui->doubleSpinBox->setMinimum(0.01f);
    ui->doubleSpinBox->setSingleStep(0.01f);

    for(int i = 0; i < MaxScreenCount;i++)
    {
        ui->comboBox->addItem(QString("%1x%2").arg(screenSize[i][0]).arg(screenSize[i][1]));
    }
    ui->comboBox->setCurrentIndex(1);

    //设置显示窗口
    ui->view->setScene(scene);
    ui->view->setRenderHint(QPainter::Antialiasing);
    ui->view->setCacheMode(QGraphicsView::CacheBackground);
    ui->view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->view->setDragMode(QGraphicsView::NoDrag);   //多选模式
    //ui->view->setDragMode(QGraphicsView::ScrollHandDrag); //小手模式
    ui->view->setAcceptDrops(true);
    connect(ui->view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect( scene, SIGNAL(selectionChanged()), this, SLOT(sceneSelectionChanged()));
    connect( scene, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(updateViewInfo()));

    this->setAcceptDrops(true);

    command = new QList<Operation*>();
}

UIEditWidget::~UIEditWidget()
{
    command->clear();
    delete command;
    delete ui;
}

void UIEditWidget::sceneSelectionChanged()
{
    View *selectedView = NULL;
    QList<QGraphicsItem *> list = scene->selectedItems();
    if(list.count() > 0)
    {
        selectedView = (View*)list.at(0);
    }else{
        selectedView = (View*)scene->topView;
    }

    showViewInfo(selectedView);
}

void UIEditWidget::showViewInfo(View *view)
{
    if(propertywidget == NULL || outlinewidget == NULL || view == NULL)
        return;

    propertywidget->initInfoForView(view);

    outlinewidget->clearSelection();
    view->QTreeWidgetItem::setSelected(true);
}

void UIEditWidget::updateViewInfo()
{
    if(propertywidget == NULL || outlinewidget == NULL)
        return;
    QList<QGraphicsItem *> list = scene->selectedItems();
    if(list.count() == 1)
    {
        View *view = (View *)list.at(0);
        propertywidget->updateInfoForView(view);
        outlinewidget->setCurrentItem(view);
    }
}

void UIEditWidget::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->view->resetMatrix();
    ui->view->scale( arg1, arg1);

    ui->viewHSlider->setValue(arg1*100);
}

void UIEditWidget::on_comboBox_currentIndexChanged(int index)
{
    scene->setSize(QSize(screenSize[index][0], screenSize[index][1]));
}

void UIEditWidget::on_viewHSlider_valueChanged(int value)
{
    float scale = value * 1.0 / 100;
    ui->view->resetMatrix();
    ui->view->scale( scale, scale);

    ui->doubleSpinBox->setValue(scale);
}

void UIEditWidget::customContextMenuRequested(QPoint point)
{
    scene->clearSelection();
    QPointF scenePoint = ui->view->mapToScene(point.x(),point.y());
    QGraphicsItem *view = scene->itemAt( scenePoint, ui->view->transform());
    if(view == NULL){
        view = (QGraphicsItem *)scene->topView;
    }

    view->setSelected(true);
    QMenu *qMenu = new QMenu(this);
    QAction* addProperty = new QAction("&隐藏",this);
    qMenu->addAction(addProperty);
    connect(addProperty, SIGNAL(triggered()), this, SLOT(changeShowState()));

    QAction* addsubview = new QAction("&添加子菜单",this);
    qMenu->addAction(addsubview);
    connect(addsubview, SIGNAL(triggered()), this, SLOT(addSubView()));

    QAction* addbview = new QAction("&添加依附与该视图",this);
    qMenu->addAction(addbview);
    connect(addbview, SIGNAL(triggered()), this, SLOT(addRelativeToView()));

    QAction* deleteview = new QAction("&删除",this);
    qMenu->addAction(deleteview);
    connect(deleteview, SIGNAL(triggered()), this, SLOT(deleteSelectedView()));
    qMenu->exec(QCursor::pos());
}

void UIEditWidget::changeShowState()
{
    if(scene->selectedItems().count() > 0){
        View *view = (View *)scene->selectedItems().at(0);
        view->setVisible(false);
        view->setCheckState(0, Qt::Unchecked);
    }
}

void UIEditWidget::addRelativeToView()
{
    View *view = (View *)outlinewidget->currentItem();
    if(view){
        NewControlDialog dialog( scene);
        if(dialog.exec() == QDialog::Accepted){
            View *sub = dialog.buildView();
            View *parent = (View *)view->parentItem();
            int addIndex = -1;
            sub->propertyAdd("relativeTo", view->getID());
            sub->propertyAdd("relativePoint", "LEFT");
            sub->propertyAdd("point", "LEFT");
            for( int i = 0; i < parent->childItems().count();i++){
                View *itr = (View*)parent->childItems().at(i);
                if(itr == view){
                     addIndex = i;
                     break;
                }
            }
            if(addIndex == -1){
                parent->addSubView(sub);
            }else{
                parent->addSubViewByIndex(addIndex+1, sub);
            }
        }
    }
}

void UIEditWidget::addSubView()
{
    View *view = (View *)outlinewidget->currentItem();
    if(view){
        NewControlDialog dialog( scene);
        if(dialog.exec() == QDialog::Accepted){
            View *sub = dialog.buildView();
            view->addSubView(sub);
            //command->append(new OperationAddItem(sub));
        }
    }
}

void UIEditWidget::deleteSelectedView()
{
    scene->clearSelection();
    View *view = (View *)outlinewidget->currentItem();
    if(view == NULL)
        return;
    QString name = view->getID();
    LOG(tr("删除")+name);
    //command->append(new OperationDeleteItem(view));
    if(view == (View *)getTopView()){
        outlinewidget->clearSelection();
        outlinewidget->takeTopLevelItem(0);
        outlinewidget->addTopLevelItem((QTreeWidgetItem *)getTopView());
    }else{
        View *parenView = (View *)view->parentItem();
        parenView->removeSubView(view);
        delete view;
        view = NULL;
    }
}

void UIEditWidget::creatScene(const QString name)
{
    scene->clear();
    scene->creatUI(name);
}

void UIEditWidget::refreshScene()
{
    QListIterator<QGraphicsItem *> items(scene->items());
    while(items.hasNext()){
        View *view = (View *)items.next();
        view->applyProperty();
    }
}

void UIEditWidget::save(const QString &filename)
{
    fileName = filename;
    if(fileName.isEmpty())
        return;
    scene->saveXML(filename);
}

void UIEditWidget::save()
{
    save(fileName);
}

bool UIEditWidget::load(QString &file)
{
    scene->readXML(file);

    return true;
}

void UIEditWidget::setShowControl(PropertyWidget *propertywidget, QTreeWidget *outlinewidget)
{
    this->outlinewidget = outlinewidget;
    this->propertywidget = propertywidget;
}

void UIEditWidget::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    if(event->key() == Qt::Key_Z && (event->modifiers() & Qt::ControlModifier)){
//        if(command->count() > 0){
//            Operation *o = command->last();
//            o->undo();
//            command->removeLast();
//            o->destroy();
//        }

    }
}

void UIEditWidget::keyReleaseEvent(QKeyEvent *event)
{
    QWidget::keyReleaseEvent(event);
}
