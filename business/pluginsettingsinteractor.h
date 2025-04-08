#ifndef PLUGINSETTINGSINTERACTOR_H
#define PLUGINSETTINGSINTERACTOR_H

#include <data/repositories/pluginsrepository.h>

class PluginsSettingsInteractor
{
public:
    PluginsSettingsInteractor();
    ~PluginsSettingsInteractor();

    bool updatePluginSettings(PluginsSettings pluginSettings, /* out */ QString& error);
    PluginsSettings getPluginSettings();
    bool rescanPluginsDir(/* out */ QString &error);
private:
    PluginsSettingsRepository *mPluginSettingsRepository;

    static bool isPluginDirPathValid(const QString &path);
    static bool isPythonPathValid(const QString &path);
};

#endif // PLUGINSETTINGSINTERACTOR_H
