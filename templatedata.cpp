#include "templatedata.h"
#include "configfile.h"
#include <QXmlStreamReader>
#include <QFile>

static TemplateData *_instance = NULL;

TemplateData::TemplateData()
{
}


TemplateData::~TemplateData()
{

}

TemplateData* TemplateData::getInstance()
{
    if(_instance == NULL)
        _instance = new TemplateData();

    return _instance;
}

bool TemplateData::loadTemplateFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QXmlStreamReader reader;
    reader.setDevice(&file);
    while(reader.readNextStartElement()){
        if(reader.name() != "xml")
        {
        }
    }
    file.close();
    if (reader.hasError()) {
        LOG(QString("Failed to parse file %1").arg(filename));
        return false;
    } else if (file.error() != QFile::NoError) {
        LOG(QString("Cannot read file %1").arg(filename));
        return false;
    }
    return true;
}
