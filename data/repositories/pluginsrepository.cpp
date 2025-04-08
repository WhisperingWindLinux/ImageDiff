#include "pluginsrepository.h"

PluginsSettingsRepository::PluginsSettingsRepository() {
    mSettings = make_unique<QSettings>("TwinPix", "PythonPluginsSettings");
}

PluginsSettings PluginsSettingsRepository::getSettings() {
    auto pythonInterpreterPath = mSettings->value("PythonInterpreterPath", "").toString();
    auto pluginsDirectoryPath = mSettings->value("PluginsDirectoryPath", "").toString();
    return { pythonInterpreterPath, pluginsDirectoryPath} ;
}

void PluginsSettingsRepository::upadteSettings(const PluginsSettings &pluginsSettings) {
    mSettings->setValue("PythonInterpreterPath", pluginsSettings.pythonInterpreterPath);
    mSettings->setValue("PluginsDirectoryPath", pluginsSettings.pluginsDirectoryPath);
}
