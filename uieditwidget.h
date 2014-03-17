#ifndef UIEDITWIDGET_H
#define UIEDITWIDGET_H

#include <QWidget>

#include "gamescene.h"

namespace Ui {
class UIEditWidget;
}

class UIEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIEditWidget(QString file, QTreeWidget *outlineWidget,  PropertyWidget *propertyWidget, QWidget *parent = 0);
    ~UIEditWidget();

    View *getTopView() { return scene->topView;}
    bool isModify() { return modify;}
    const QString getFileName() { return fileName;}
    GameScene *scene;

    void showViewInfo(View *view);
    void refreshScene();

    void save();
    void save(const QString &filename);
public slots:
    void addRelativeToView();
    void addSubView();
    void deleteSelectedView();
private slots:
    void on_doubleSpinBox_valueChanged(double arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_viewHSlider_valueChanged(int value);

    void sceneSelectionChanged();

    void updateViewInfo();

    void customContextMenuRequested(QPoint point);

    void changeShowState();

private:
    PropertyWidget *propertywidget;

    QTreeWidget *outlinewidget;
    Ui::UIEditWidget *ui;
    QString fileName;
    bool modify;
};

#endif // UIEDITWIDGET_H
