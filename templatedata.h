#ifndef TEMPLATEDATA_H
#define TEMPLATEDATA_H
#include <qt>
#include <QMap>
#include <QString>

typedef struct _Property
{
    QString name;
    QString type;
    QString value;
}MyProperty;


class TemplateData
{
public:
    static TemplateData* getInstance();
    bool loadTemplateFile(const QString &filename);
private:
    TemplateData();
    ~TemplateData();

    QMap<QString, MyProperty*> *templateMap;
};

#endif // TEMPLATEDATA_H
