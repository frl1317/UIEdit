#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "propertywidget.h"
#include <QMainWindow>
#include <QFileSystemModel>

#include "gamescene.h"

#include <QTreeWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addRelativeToView();
    void addSubView();
    void deleteSelectedView();
private slots:
    void on_action_setingResource_triggered();

    void on_actionRefresh_triggered();

    void treeViewClicked(const QModelIndex &index);

    void treeItemChanged(QTreeWidgetItem *item, int column);
    void propertyChanged(QtProperty* property,QVariant variant);
    void attributeChanged(QtProperty * _t1, const QString & _t2, const QVariant & _t3);
    void clickedOutlineTreeItem(QModelIndex modelIndex);
    void treeWidgetContextMenu(QPoint point);
    void treeViewContextMenu(QPoint point);

    void on_action_savexml_triggered();

    //tab event
    void closeAllWindow();
    void tabCloseRequested();
    void tabCloseRequested(int index);
    void tabCloseRequested(int index, int defaultres);
    void customContextMenuRequested(QPoint point);

    void currentChanged(int index);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void on_action_closeall_triggered();

    void newUIXML();
    void newLua();
    void deleteFile();

    void showPrint(bool print);
    void showProperty(bool b);
    void showControls(bool b);
    void on_action_help_triggered();
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QFileSystemModel model;

    //PropertyWidget propertybrowser;

    void saveIni();
    void loadIni();
    void setResourcePath(const QString &path);

    View *copyView;
};

#endif // MAINWINDOW_H
