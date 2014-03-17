#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include <QT>
#include <QString>
#include <QDebug>
#include "printfdockwidget.h"

#define LOG(s) PrintfDockWidget::getInstance()->printf(s);
#define getConfigFile() ConfigFile::getInstance()

class ConfigFile
{
public:
    QString getResourcesPath(){
        return resourcesPath;
    }
    QString getFullPath(const QString& filename){
        return getConfigFile()->getResourcesPath() + "//" + filename;
    }

    void setResourcesPath(const QString& path){resourcesPath = path;}

    static ConfigFile *getInstance();
private:
    ConfigFile();
    QString resourcesPath;
};

#endif // CONFIGFILE_H
