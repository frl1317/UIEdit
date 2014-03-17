#include "configfile.h"

static ConfigFile *configFile = NULL;

ConfigFile::ConfigFile()
{
}

ConfigFile *ConfigFile::getInstance()
{
    if(configFile == NULL)
        configFile = new ConfigFile();

    return configFile;
}
