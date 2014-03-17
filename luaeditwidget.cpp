#include "luaeditwidget.h"
#include "ui_luaeditwidget.h"
#include <QFile>
#include <QDialog>
#include "configfile.h"

LuaEditWidget::LuaEditWidget(QString file, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LuaEditWidget)
{
    ui->setupUi(this);

    luaedit = new uni::ULuaEdit();
    layout()->addWidget(luaedit);
    readLuaFile(file);
}

LuaEditWidget::~LuaEditWidget()
{
    delete luaedit;
    delete ui;
}

bool LuaEditWidget::readLuaFile(QString luafile)
{
    this->luafile = luafile;
    if(luafile.isEmpty())
        return false;

    QFile file(luafile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    //luaedit->insertPlainText(file.readAll());
    luaedit->setPlainText(file.readAll());
    file.close();
    return true;
}

bool LuaEditWidget::save(const QString& filename)
{
    luafile = filename;
    QFile file(luafile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }

    file.write(luaedit->toPlainText().toUtf8());
    file.close();
    return true;
}

bool LuaEditWidget::save()
{
    if(luafile.isEmpty())
        return false;

    return save(luafile);
}

void LuaEditWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "11111";
    if (event->key() == Qt::Key_F && (event->modifiers() & Qt::ControlModifier))
    {
        qDebug() << "22222";
        QDialog *dlg = new QDialog(this);
        dlg->exec();
    }else{
        QWidget::keyPressEvent(event);
    }
}
