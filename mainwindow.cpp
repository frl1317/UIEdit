#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"
#include <QDragEnterEvent>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QPropertyAnimation>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include "configfile.h"
#include "newcontroldialog.h"
#include "newpropertydialog.h"
#include "newfile.h"
#include "controlspaintwidget.h"
#include "uieditwidget.h"
#include "luaeditwidget.h"
#include "qfileinfo.h"
#include <QMessageBox>
#include "uixml.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    copyView(NULL)
{
    ui->setupUi(this);

    //model.setFilter(QDir::AllDirs);
    model.setNameFilters( QStringList() << "*.xml");// << "*.lua"
    model.setNameFilterDisables(false);
    ui->fileTreeView->setModel(&model);
    ui->fileTreeView->setColumnHidden( 0, true);
    ui->fileTreeView->setColumnHidden( 1, true);
    ui->fileTreeView->setColumnHidden( 2, true);
    ui->fileTreeView->setColumnHidden( 3, true);
    connect(ui->fileTreeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(treeViewClicked(const QModelIndex &)));
    connect(ui->fileTreeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(treeViewContextMenu(QPoint)));

    loadIni();

    PropertyWidget *propertybrowser = PropertyWidget::getInstance();
    ui->dockWidget_2->setWidget(propertybrowser);
    connect( propertybrowser->variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(propertyChanged(QtProperty*,QVariant)));
    connect( propertybrowser->variantManager, SIGNAL(attributeChanged(QtProperty *, const QString, const QVariant)), this, SLOT(attributeChanged(QtProperty *, const QString, const QVariant)));

    //控制台输出
    PrintfDockWidget *dockWidget = PrintfDockWidget::getInstance();
    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);

    //初始化大纲
    QStringList header;
    header << "对象" << "类型";
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setAlternatingRowColors(true);
    ui->treeWidget->setHeaderLabels(header);
    ui->treeWidget->setColumnWidth( 0, 200);
    connect( ui->treeWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedOutlineTreeItem(QModelIndex)));
    connect( ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(treeWidgetContextMenu(QPoint)));
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(treeItemChanged(QTreeWidgetItem*, int)));

    ui->listWidget->setIconSize(QSize(60,60));
    ui->listWidget->setDragEnabled(true);
    QSetIterator<QtProperty *> itr(propertybrowser->group->properties());
    QStringList list;
    while(itr.hasNext()){
        QtProperty *pro = itr.next();
        list << pro->propertyName();
    }
    list.sort();
    for( int i = 0; i < list.count(); i++){
        QString str = list.at(i);
        QListWidgetItem *item = new QListWidgetItem();
        ui->listWidget->addItem(item);

        ControlsPaintWidget *widget = new ControlsPaintWidget(ui->listWidget, "", str, "描述");
        ui->listWidget->setItemWidget(item, widget);
        item->setSizeHint (QSize(widget->rect().width(),widget->rect().height()));
    }

    setWindowState(Qt::WindowMaximized);

    ui->tabWidget->clear();
    connect( ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
    connect( ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));

    ui->actionShowPrint->setChecked(true);
    ui->actionShowPrint->setCheckable(true);
    connect( ui->actionShowPrint, SIGNAL(triggered(bool)), this, SLOT(showPrint(bool)));

    ui->actionShowProperty->setChecked(true);
    ui->actionShowProperty->setCheckable(true);
    connect( ui->actionShowProperty, SIGNAL(triggered(bool)), this, SLOT(showProperty(bool)));

    ui->actionShowControls->setChecked(true);
    ui->actionShowControls->setCheckable(true);
    connect( ui->actionShowControls, SIGNAL(triggered(bool)), this, SLOT(showControls(bool)));

    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
    PropertyWidget::getInstance()->initInfoForView(NULL);
    PropertyWidget::getInstance()->isEnd = true;

    while(ui->tabWidget->count()>0){
        tabCloseRequested(0, 1);
    }
    delete ui;

//    if(copyView)
//        delete copyView;
}

void MainWindow::showProperty(bool b)
{
    ui->dockWidget_2->setVisible(b);
}

void MainWindow::showControls(bool b)
{
    ui->dockWidget->setVisible(b);
}
void MainWindow::showPrint(bool print)
{
    PrintfDockWidget::getInstance()->setVisible(print);
}

void MainWindow::treeItemChanged(QTreeWidgetItem *item, int column)
{
    if(item){
        View *view = (View *)item;
        if(view->getPropertyByKey("hide") == "true"){
            view->setVisible(false);
        }else{
            view->setVisible(item->checkState(0));
        }
    }
}

void MainWindow::clickedOutlineTreeItem(QModelIndex modelIndex)
{
    UIEditWidget *widget = (UIEditWidget *)ui->tabWidget->currentWidget();
    widget->scene->clearSelection();

    View *view = (View *)ui->treeWidget->currentItem();
    if(view && view->isVisible()){
        view->QGraphicsItem::setSelected(true);
    }
    //view->setExpanded(true);
    widget->showViewInfo(view);
}

void MainWindow::treeWidgetContextMenu(QPoint point)
{
    View *view = (View *)ui->treeWidget->currentItem();
    if(view == NULL)
        return;
    if(view->isVisible()){
        view->QGraphicsItem::setSelected(true);
    }

    QMenu *qMenu = new QMenu(ui->treeWidget);

    QAction* addSubView = new QAction("&添加子视图",this);
    qMenu->addAction(addSubView);
    connect(addSubView, SIGNAL(triggered()), this, SLOT(addSubView()));

    QAction* addView = new QAction("&添加依附与该视图",this);
    qMenu->addAction(addView);
    connect(addView, SIGNAL(triggered()), this, SLOT(addRelativeToView()));

    QAction* removeView = new QAction("&删除视图",this);
    qMenu->addAction(removeView);
    connect(removeView, SIGNAL(triggered()), this, SLOT(deleteSelectedView()));

    qMenu->exec(QCursor::pos());
}

void MainWindow::addRelativeToView()
{
    UIEditWidget *uiedit =  (UIEditWidget *)ui->tabWidget->currentWidget();
    uiedit->addRelativeToView();
}

void MainWindow::addSubView()
{
    UIEditWidget *uiedit =  (UIEditWidget *)ui->tabWidget->currentWidget();
    uiedit->addSubView();
}

void MainWindow::deleteSelectedView()
{
    UIEditWidget *uiedit =  (UIEditWidget *)ui->tabWidget->currentWidget();
    uiedit->deleteSelectedView();
}

void MainWindow::propertyChanged(QtProperty* property,QVariant variant)
{
    PropertyWidget *propertybrowser = PropertyWidget::getInstance();
    if(propertybrowser->currentView != NULL){
        if(propertybrowser->currentView->propertyContains(property->propertyName()))
            propertybrowser->currentView->propertyUpdate(property->propertyName(), variant.toString());
        else
            propertybrowser->currentView->propertyAdd(property->propertyName(), variant.toString());
    }
}

void MainWindow::attributeChanged(QtProperty * _t1, const QString & _t2, const QVariant & _t3)
{
    qDebug()<< "attributeChanged" << _t1->propertyName() << "==>" << _t2;
    PropertyWidget *propertybrowser = PropertyWidget::getInstance();
    if(propertybrowser->currentView != NULL){
        //qDebug() << property->propertyName() << "==>" <<  variant.toString();
//        LOG(property->propertyName()+"==>"+variant.toString());
//        if(propertybrowser->currentView->propertyContains(property->propertyName()))
//            propertybrowser->currentView->propertyUpdate(property->propertyName(), variant.toString());
//        else
//            propertybrowser->currentView->propertyAdd(property->propertyName(), variant.toString());
    }
}

void MainWindow::saveIni()
{
    QString path = QDir::tempPath() + "/" + "game.ini";
    QFile file(path);
    if(file.open(QFile::ReadWrite|QFile::Text))
    {
        file.write(ConfigFile::getInstance()->getResourcesPath().toUtf8());
        file.close();
    }
}

void MainWindow::loadIni()
{
    QString path = QDir::tempPath() + "/" + "game.ini";
    QFile file(path);
    if(file.open(QFile::ReadWrite|QFile::Text))
    {
        setResourcePath(file.readAll().data());
        file.close();
    }
}

void MainWindow::on_action_setingResource_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Seting Image"), ConfigFile::getInstance()->getResourcesPath());
    if(path.isEmpty() == false){
        setResourcePath(path);
    }
}

void MainWindow::setResourcePath(const QString &path)
{
    LOG("配置资源目录:" + path);
    ConfigFile::getInstance()->setResourcesPath(path);
    ui->fileTreeView->setRootIndex(model.setRootPath(path));
    ui->fileTreeView->setColumnHidden( 0, false);
    saveIni();
}

void MainWindow::on_actionRefresh_triggered()
{
    UIEditWidget *uiedit =  (UIEditWidget *)ui->tabWidget->currentWidget();
    if(uiedit)
        uiedit->refreshScene();
}

void MainWindow::on_action_savexml_triggered()
{
    QWidget *widget = ui->tabWidget->currentWidget();
    if(widget)
    {
        if(widget->windowTitle().contains(".xml"))
        {
            UIEditWidget *uiedit =  (UIEditWidget *)widget;
            uiedit->save();
            LOG("保存" + uiedit->windowTitle());
        }else{
            LuaEditWidget *luaedit =  (LuaEditWidget *)widget;
            luaedit->save();
            LOG("保存" + luaedit->windowTitle());
        }
    }
}

void MainWindow::newLua()
{
    NewFile dlg(this);
    if(dlg.exec() == QDialog::Accepted){
        LuaEditWidget *luaedit = new LuaEditWidget();
        QString path = ConfigFile::getInstance()->getResourcesPath();
        path = path + ui->fileTreeView->currentIndex().data().toString();//获得当前右击事件的节点数据
        path = path +"/"+dlg.filename() + ".lua";
        luaedit->save(path);

        QFileInfo info(path);
        luaedit->setWindowTitle(path);
        ui->tabWidget->addTab(luaedit, info.fileName());
        ui->tabWidget->setCurrentWidget(luaedit);
    }
}

void MainWindow::newUIXML()
{
    NewFile dlg(this);
    if(dlg.exec() == QDialog::Accepted){
        PropertyWidget *propertybrowser =PropertyWidget::getInstance();
        QString path = ConfigFile::getInstance()->getResourcesPath();
        path = path + "/" + ui->fileTreeView->currentIndex().data().toString();//获得当前右击事件的节点数据
        path = path +"/"+dlg.filename() + ".xml";
        UIEditWidget *uiedit = new UIEditWidget(this);
        uiedit->setShowControl( propertybrowser, ui->treeWidget);
        uiedit->creatScene(dlg.filename());
        uiedit->save(path);

        QFileInfo info(path);
        uiedit->setWindowTitle(path);
        ui->tabWidget->addTab(uiedit, info.fileName());
        ui->tabWidget->setCurrentWidget(uiedit);
    }
}

void MainWindow::deleteFile()
{
    QModelIndex index = ui->fileTreeView->currentIndex();
    if (model.fileInfo(index).isDir())
    {
        //删除目录
    } else{
        model.remove(index);
    }
    LOG("删除文件"+ConfigFile::getInstance()->getResourcesPath()+index.data().toString());
}

void MainWindow::treeViewClicked(const QModelIndex &index)
{
    QString file = model.filePath(index);
    QFileInfo info(file);
    PropertyWidget *propertybrowser = PropertyWidget::getInstance();
    QWidget *widget = NULL;
    LOG("打开文件" + model.filePath(index));
    if(file.contains(".xml") && file.contains("ui/")){  //ui-xml
        UIEditWidget *uiedit = new UIEditWidget(this);
        uiedit->setShowControl( propertybrowser, ui->treeWidget);
        uiedit->load(file);
        widget = uiedit;
        ui->treeWidget->takeTopLevelItem(0);
        ui->treeWidget->addTopLevelItem((QTreeWidgetItem *)uiedit->getTopView());
    }else if(file.contains(".lua") || (file.contains(".xml") && file.contains("data/"))){          //lua
        ui->treeWidget->takeTopLevelItem(0);
        propertybrowser->clear();
        widget = new LuaEditWidget(file, this);
    }

    if(widget){
        widget->setWindowTitle(file);
        ui->tabWidget->addTab(widget, info.fileName());
        ui->tabWidget->setCurrentWidget(widget);
    }
}

void MainWindow::currentChanged(int index)
{
    PropertyWidget::getInstance()->clear();
    QWidget *widget = ui->tabWidget->widget(index);
    ui->treeWidget->takeTopLevelItem(0);
    if(widget && widget->windowTitle().contains(".xml") && widget->windowTitle().contains("ui/")){
        UIEditWidget *uiedit = (UIEditWidget *)widget;
        QTreeWidgetItem *topLevelItem = (QTreeWidgetItem *)uiedit->getTopView();
        ui->treeWidget->addTopLevelItem(topLevelItem);
    }
}
void MainWindow::closeAllWindow()
{
    if(ui->tabWidget->count() > 0){
        int res = QMessageBox::question(this, "文件未保存", tr("是否保存所有文件?"), tr("全部保存"), tr("不保存"), tr("取消"));
        while(ui->tabWidget->count()>0 && res != 2){
            tabCloseRequested(0, res);
        }
    }
}

void MainWindow::tabCloseRequested()
{
    if(ui->tabWidget->currentWidget()){
        tabCloseRequested(ui->tabWidget->currentIndex(), -1);
    }
}

void MainWindow::tabCloseRequested(int index)
{
    tabCloseRequested(index, -1);
}

void MainWindow::tabCloseRequested(int index, int defaultres)
{
    UIEditWidget *widget = (UIEditWidget*)ui->tabWidget->widget(index);
    QString windowTitle = widget->windowTitle();
    if(windowTitle.contains(".xml") && windowTitle.contains("ui/")){
        int res = defaultres;
        if(res == -1){
            res = QMessageBox::question(this, "文件未保存", widget->getFileName()+tr("是否保存文件?"), tr("保存"), tr("不保存"), tr("取消"));
        }
        switch(res){
        case 0:
            widget->save();
            ui->tabWidget->removeTab(index);
            delete widget;
            widget = NULL;
            break;
        case 1:
            LOG("关闭"+widget->getFileName());
            ui->tabWidget->removeTab(index);
            delete widget;
            widget = NULL;
            break;
        case 2:
            return;
        }
    }else  if(windowTitle.contains(".lua")){
        LuaEditWidget *widget = (LuaEditWidget *)ui->tabWidget->widget(index);
        int res = defaultres;
        if(res == -1){
            res = QMessageBox::question(this, "文件未保存",tr("是否保存文件?"), tr("保存"), tr("不保存"), tr("取消"));
        }
        switch(res){
        case 0:
            widget->save();
            ui->tabWidget->removeTab(index);
            delete widget;
            widget = NULL;
            break;
        case 1:
            ui->tabWidget->removeTab(index);
            delete widget;
            widget = NULL;
            break;
        case 2:
            return;
        }
    }else{
        QWidget *widget = ui->tabWidget->widget(index);
        delete widget;
        widget = NULL;
    }

}

void MainWindow::customContextMenuRequested(QPoint point)
{
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier))
    {
        View *view = (View *)ui->treeWidget->currentItem();
        if(view){
            if(copyView != view)
            {
                if(copyView){
                    delete copyView;
                }
                copyView = view->copyView();
                LOG("拷贝"+copyView->getClassName() + ":" + copyView->getID());
            }
        }
    }
    else if (event->key() == Qt::Key_V && (event->modifiers() & Qt::ControlModifier))
    {
        if(copyView){
            LOG("粘贴"+copyView->getClassName() + ":" + copyView->getID());
            View *parenview = (View *)ui->treeWidget->currentItem();
            if(parenview){
                View *view = copyView;
                parenview->addSubView(view);
                UIEditWidget *uiedit = (UIEditWidget*)ui->tabWidget->currentWidget();
                uiedit->refreshScene();
                copyView = NULL;
            }
        }
    }
    else if (event->key() == Qt::Key_S && (event->modifiers() & Qt::ControlModifier))
    {
        on_action_savexml_triggered();
    }else if(event->key() == Qt::Key_F5){
        on_actionRefresh_triggered();
    }else if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace){
        deleteSelectedView();
    }
    else if(event->key() == Qt::Key_F && (event->modifiers() & Qt::ControlModifier)){
        qDebug() << "查找";
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QWidget::keyReleaseEvent(event);
}

void MainWindow::on_action_closeall_triggered()
{
    closeAllWindow();
}

void MainWindow::treeViewContextMenu(QPoint point)
{
    QModelIndex index = ui->fileTreeView->currentIndex();
    if(index.isValid() == false)
        return;
    QMenu *qMenu = new QMenu(ui->fileTreeView);
    if (model.fileInfo(index).isDir())
    {
        QAction* action1 = new QAction("&新建uixml",this);
        qMenu->addAction(action1);
        connect(action1, SIGNAL(triggered()), this, SLOT(newUIXML()));//showCurve为点击菜单右键“菜单显示”的槽函数

        QAction* action2 = new QAction("&新建lua",this);
        qMenu->addAction(action2);
        connect(action2, SIGNAL(triggered()), this, SLOT(newLua()));//showCurve为点击菜单右键“菜单显示”的槽函数
        qMenu->exec(QCursor::pos());
    } else{
        QAction* action1 = new QAction("&删除",this);
        qMenu->addAction(action1);
        connect(action1, SIGNAL(triggered()), this, SLOT(deleteFile()));//showCurve为点击菜单右键“菜单显示”的槽函数
        qMenu->exec(QCursor::pos());
    }
}

void MainWindow::on_action_help_triggered()
{
    QMessageBox::information(NULL, tr("Error"), tr("1.首先配置项目工作目录,需要设置到资源目录(包括,ui，data).\n2.开始工作吧.\n\n注意!修改界面后请先clear下xcode项目工程!否则xcode不会更新资源!"));
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
//    if(arg1.isEmpty())
//    {
//        model.setNameFilters( QStringList() << "*.xml");// << "*.lua"
//        model.setFilter(QDir::AllEntries);
//    }else{
//        model.setNameFilters( QStringList() << arg1+"*.xml");// << "*.lua"
//        model.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
//    }
}
