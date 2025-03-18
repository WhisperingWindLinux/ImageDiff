#ifndef PLUGINSETTINGSINTERACTOR_H
#define PLUGINSETTINGSINTERACTOR_H

#include <data/storage/repositories/pluginsrepository.h>

using namespace std;

class PluginsSettingsInteractor
{
public:
    PluginsSettingsInteractor();

    bool updatePluginSettings(PluginsSettings pluginSettings, /* out */ QString& error);
    PluginsSettings getPluginSettings();
    bool rescanPluginsDir(/* out */ QString &error);
private:
    unique_ptr<PluginsSettingsRepository> pluginSettingsRepository;

    static bool isPluginDirPathValid(const QString &path);
    static bool isPythonPathValid(const QString &path);
};

#endif // PLUGINSETTINGSINTERACTOR_H
