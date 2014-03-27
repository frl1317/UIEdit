#-------------------------------------------------
#
# Project created by QtCreator 2013-08-29T14:02:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UIEdit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gamescene.cpp \
    timeline.cpp \
    configfile.cpp \
    view.cpp \
    printfdockwidget.cpp \
    image.cpp \
    group.cpp \
    label.cpp \
    button.cpp \
    windows.cpp \
    propertywidget.cpp \
    propgrid.cpp \
    radiobutton.cpp \
    richtext.cpp \
    scrollview.cpp \
    uixml.cpp \
    newcontroldialog.cpp \
    newpropertydialog.cpp \
    uieditwidget.cpp \
    luaeditwidget.cpp \
    LuaEdit/ULuaEdit.cpp \
    propertyeditdockwidget.cpp \
    newfile.cpp \
    controlspaintwidget.cpp \
    templatedata.cpp \
    functionclass.cpp \
    operation.cpp \
    checkbox.cpp

HEADERS  += mainwindow.h \
    gamescene.h \
    timeline.h \
    configfile.h \
    view.h \
    printfdockwidget.h \
    image.h \
    group.h \
    label.h \
    button.h \
    windows.h \
    propertywidget.h \
    propgrid.h \
    radiobutton.h \
    richtext.h \
    scrollview.h \
    uixml.h \
    newcontroldialog.h \
    newpropertydialog.h \
    uieditwidget.h \
    luaeditwidget.h \
    LuaEdit/ULuaEdit.h \
    propertyeditdockwidget.h \
    newfile.h \
    controlspaintwidget.h \
    templatedata.h \
    functionclass.h \
    operation.h \
    checkbox.h

FORMS    += mainwindow.ui \
    publicproperties.ui \
    timeline.ui \
    printfdockwidget.ui \
    newcontroldialog.ui \
    newpropertydialog.ui \
    uieditwidget.ui \
    luaeditwidget.ui \
    propertyeditdockwidget.ui \
    newfile.ui \
    controlspaintwidget.ui

RESOURCES += \
    Resources.qrc

include(./qtpropertybrowser/qtpropertybrowser.pri)

ICON = app.icns

SUBDIRS += \
    qt-qtmacextras/qtmacextras.pro
