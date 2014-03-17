#include "propertywidget.h"
#include <QDebug>
#include "view.h"
#include "button.h"
#include "label.h"
#include "image.h"
#include <QSettings>
#include <QFile>
#include <QMessageBox>

PropertyWidget *__instance = NULL;

PropertyWidget* PropertyWidget::getInstance()
{
    if(__instance == NULL)
        __instance = new PropertyWidget();

    return __instance;
}
PropertyWidget::PropertyWidget()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    variantManager = new QtVariantPropertyManager();   //属性管理器
    factory = new QtVariantEditorFactory();    //属性编辑器
    group = new QtGroupPropertyManager();      //多个属性管理器
    setFactoryForManager(variantManager, factory);
    classMap = new QMap<QString, QtProperty*>();
    enumManager = new QtEnumPropertyManager();

    loadTemplate("://Resources/Template.xml");
    isEnd = false;
//    connect( variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(propertyChange(QtProperty*,QVariant)));
//    connect( this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(propertyContextMenu(QPoint)));

}

PropertyWidget::~PropertyWidget()
{
    propertyValueBack.clear();
    parentClassValueBack.clear();
    delete variantManager;
    delete factory;
    delete group;
    delete classMap;
}
void skipUnknownElement(QXmlStreamReader &reader)
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

bool PropertyWidget::loadTemplate(const QString &xmlFile)
{
    QXmlStreamReader reader;
    QFile file(xmlFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(NULL, tr("Error"),
                              tr("Cannot read file %1").arg(xmlFile));
        return false;
    }

    reader.setDevice(&file);

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {  //template
            loadSubProperty(reader);
        }else{
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

bool PropertyWidget::loadSubProperty(QXmlStreamReader &reader)
{
    reader.readNext();//加载过xml---必须有
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            QString name = reader.name().toString().toLower();
            QtProperty *pp = group->addProperty(name);
            loadSubAttribute(reader, pp);
            classMap->insert(name, pp);
        }else{
            reader.readNext();
        }
    }
    return true;
}

void PropertyWidget::loadSubAttribute(QXmlStreamReader &reader, QtProperty *property)
{
    QtVariantProperty *vp = NULL;
    reader.readNext();
    while (!reader.atEnd()) {
        if(reader.isEndElement()){
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            if(reader.name() == "parent"){
                QString value = reader.attributes().value("class").toString().toLower();
                parentClassValueBack.insert( property->propertyName().toLower(), value);
            }else if (reader.name() == "property") {
                QString name = reader.attributes().value("name").toString();
                QString value = reader.attributes().value("value").toString();
                QString type = reader.attributes().value("type").toString().toLower();
                if(type == "int"){
                    vp = variantManager->addProperty(QVariant::Int, name);
                }else if(type == "uint"){
                    vp = variantManager->addProperty(QVariant::UInt, name);
                }else if(type == "string"){
                    vp = variantManager->addProperty(QVariant::String, name);
                }else if(type == "float" || type == "double"){
                    vp = variantManager->addProperty(QVariant::Double, name);
                }else if(type == "bool"){
                    vp = variantManager->addProperty(QVariant::Bool, name);
                }else if(type == "textlength"){
                    vp = variantManager->addProperty(QVariant::TextLength, name);
                }else{
                    vp = variantManager->addProperty(QVariant::String, name);
                }
                if(vp){
                    vp->setValue(value);
                    property->addSubProperty(vp);
                }

                propertyValueBack.insert(name, new TemplateProperty(name, type, value));
            }
            skipUnknownElement(reader);
        }else{
            reader.readNext();
        }
    }
}

void PropertyWidget::initPropertyValue(QtProperty *groupProperty)
{
    QtVariantProperty *vp = NULL;
    QListIterator<QtProperty *> itr(groupProperty->subProperties());
    while(itr.hasNext()){
        QtProperty *pp = itr.next();
        vp = (QtVariantProperty *)pp;
        TemplateProperty *p = propertyValueBack.find(pp->propertyName()).value();
        vp->setValue(p->value);
    }
}

void PropertyWidget::addParentProperty(QString className)
{
    QMap<QString, QString>::Iterator itr = parentClassValueBack.find(className);
    if(itr != parentClassValueBack.end()){
        QMap<QString, QtProperty*>::Iterator itr2 = classMap->find(itr.value());
        if(itr2 != classMap->end()){
            QtProperty* p = itr2.value();
            addParentProperty(p->propertyName());
            initPropertyValue(p);
            addProperty(p);
        }
    }
}

void PropertyWidget::initInfoForView(View *view)
{
    if(isEnd)
        return;
    currentView = NULL;
    this->clear();
    if(view == NULL)
        return;

    QtVariantProperty* vp = variantManager->addProperty(QVariant::String, "ClassName");
    vp->setValue(view->getClassName());
    vp->setEnabled(false);
    addProperty(vp);

    addParentProperty(view->getClassName());

    QMap<QString, QtProperty*>::Iterator itr2 = classMap->find(view->getClassName());
    if(itr2 != classMap->end()){
        addProperty(*itr2);
        initPropertyValue(itr2.value());
    }

    currentView = view;
    updateInfoForView(view);
}

void PropertyWidget::updateInfoForView(View *view)
{
    if(isEnd)
        return;
    if(view == NULL)
        return;

    QList<MyProperty *>::Iterator itr = view->propertys.begin();
    for(; itr != view->propertys.end(); ++itr)
    {
        MyProperty *p = *itr;
        QtVariantProperty* vp = (QtVariantProperty*)findPropertyByName(view->getClassName(), p->name);
        if(vp){
            vp->setValue(p->value);
        }
    }
}

QtProperty* PropertyWidget::findPropertyByName(const QString className, const QString &propertyname)
{
    QSetIterator<QtProperty *> itr(group->properties());
    //筛选类别
    while(itr.hasNext()){
        QtProperty *property = itr.next();
        bool isFind = false;
        if(property->propertyName() == className){
            isFind = true;
        }else{
            QMap<QString, QString>::Iterator itr2 = parentClassValueBack.find(className);
            if(itr2 != parentClassValueBack.end())
            {
                QString parentClassName = itr2.value();
                if(property->propertyName() == parentClassName)
                    isFind = true;
            }
        }
        if(isFind == false)
            continue;
        //查找属性
        for( int i = 0; i < property->subProperties().count(); ++i){
            QtProperty *sub = property->subProperties().at(i);
            if(sub->propertyName() == propertyname){
                return sub;
            }
        }
    }
    return NULL;
}

QString PropertyWidget::getPropertyValueByName(const QString &propertyname)
{
    if( propertyname == NULL || propertyname.isNull() || propertyname.isEmpty())
        return NULL;
    QMap<QString, TemplateProperty*>::Iterator itr = propertyValueBack.find(propertyname);
    for(; itr != propertyValueBack.end(); ++itr)
    {
        TemplateProperty *p = itr.value();
        return p->value;
    }
    return NULL;
}
