#ifndef PLUGINSETTINGSINTERACTOR_H
#define PLUGINSETTINGSINTERACTOR_H

#include <data/storage/repositories/pluginsrepository.h>

using namespace std;

class PluginSettingsInteractor
{
public:
    PluginSettingsInteractor();

    bool updatePluginSettings(PluginsSettings pluginSettings, QString& error);
    PluginsSettings getPluginSettings();

private:
    unique_ptr<PluginsSettingsRepository> pluginSettingsRepository;

    static bool isPluginDirPathValid(const QString &path);
    static bool isPythonPathValid(const QString &path);
};

#endif // PLUGINSETTINGSINTERACTOR_H
