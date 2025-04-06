#include "pluginsrepository.h"

PluginsSettingsRepository::PluginsSettingsRepository() {
    settings = make_unique<QSettings>("TwinPix", "PythonPluginsSettings");
}

PluginsSettings PluginsSettingsRepository::getSettings() {
    auto pythonInterpreterPath = settings->value("PythonInterpreterPath", "").toString();
    auto pluginsDirectoryPath = settings->value("PluginsDirectoryPath", "").toString();
    return { pythonInterpreterPath, pluginsDirectoryPath} ;
}

void PluginsSettingsRepository::upadteSettings(const PluginsSettings &pluginsSettings) {
    settings->setValue("PythonInterpreterPath", pluginsSettings.pythonInterpreterPath);
    settings->setValue("PluginsDirectoryPath", pluginsSettings.pluginsDirectoryPath);
}
