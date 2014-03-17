#ifndef LUAEDITWIDGET_H
#define LUAEDITWIDGET_H

#include <QWidget>
#include "LuaEdit/ULuaEdit.h"

namespace Ui {
class LuaEditWidget;
}

class LuaEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LuaEditWidget(QString file = 0, QWidget *parent = 0);
    ~LuaEditWidget();

    bool readLuaFile(QString luafile);

    bool save(const QString& filename);
    bool save();
private:
    void keyPressEvent(QKeyEvent *event);

    Ui::LuaEditWidget *ui;
    uni::ULuaEdit *luaedit;
    QString luafile;
};

#endif // LUAEDITWIDGET_H
