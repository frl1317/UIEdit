#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include "QtTreePropertyBrowser.h"
#include "qtvariantproperty.h"
#include "qtpropertymanager.h"
#include <QList>
#include <QMap>

class TemplateProperty
{
public:
    TemplateProperty(QString name, QString type, QString value)
        :name(name),
         type(type),
         value(value)
    {
    }
    QString name;
    QString type;
    QString value;
};

class TemplateClass
{
public:
    TemplateClass(){}
    QString name;
    QList<TemplateProperty*> *propertys;
    QList<TemplateClass*> *parents;
};

class View;
class QSettings;
class QXmlStreamReader;

class PropertyWidget : public QtTreePropertyBrowser
{
public:
    static PropertyWidget* getInstance();
    bool loadTemplate(const QString &xmlFile);
    bool loadSubProperty(QXmlStreamReader &reader);
    void loadSubAttribute(QXmlStreamReader &reader, QtProperty *property);
    void parseSubAttribute(QXmlStreamReader &reader);

    void addParentProperty(QString className);
    void initPropertyValue(QtProperty *groupProperty);
    void initInfoForView(View *view);
    void updateInfoForView(View *view);
    QtProperty* findPropertyByName(const QString className, const QString &propertyname);
    QString getPropertyValueByName(const QString &propertyname);
    bool isModification(const QString &propertyname);

    QtVariantPropertyManager *variantManager;   //属性管理器
    View *currentView;
    QtVariantEditorFactory *factory;    //属性编辑器
    QtGroupPropertyManager *group;      //多个属性管理器
    QtEnumPropertyManager *enumManager; //枚举管理器
    QMap<QString, TemplateProperty*> propertyValueBack;
    QMap<QString, QString> parentClassValueBack;

    QMap<QString, QtProperty*> *classMap;
    bool isEnd;
private:
    PropertyWidget();
    ~PropertyWidget();
};

#endif // PROPERTYWIDGET_H
