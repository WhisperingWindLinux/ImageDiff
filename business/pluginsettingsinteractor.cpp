#include "pluginsettingsinteractor.h"

#include <qfileinfo.h>

PluginSettingsInteractor::PluginSettingsInteractor() {
    pluginSettingsRepository = make_unique<PluginsSettingsRepository>();
}

bool PluginSettingsInteractor::updatePluginSettings(PluginsSettings pluginSettings, QString& error) {
    if (!isPythonPathValid(pluginSettings.pythonInterpreterPath)) {
        error.append(QString("Bad python interpreter."));
        return false;
    }
    if (!isPluginDirPathValid(pluginSettings.pluginsDirectoryPath)) {
        error.append(QString("Incorrect plugins directory path."));
        return false;
    }
    pluginSettingsRepository->upadteSettings(pluginSettings);
    return true;
}

PluginsSettings PluginSettingsInteractor::getPluginSettings() {
    auto pluginSettings = pluginSettingsRepository->getSettings();

    QString pythonInterpreterPath;
    QString pluginsDirectoryPath;

    if (isPythonPathValid(pluginSettings.pythonInterpreterPath)) {
        pythonInterpreterPath = pluginSettings.pythonInterpreterPath;
    }
    if (isPluginDirPathValid(pluginSettings.pluginsDirectoryPath)) {
        pluginsDirectoryPath = pluginSettings.pluginsDirectoryPath;
    }
    return {pythonInterpreterPath, pluginsDirectoryPath};
}

bool PluginSettingsInteractor::isPluginDirPathValid(const QString &path) {
    QFileInfo pluginsDirInfo { path };
    if (!pluginsDirInfo.exists() ||
        !pluginsDirInfo.isDir()) {
        return false;
    }
    return true;
}

bool PluginSettingsInteractor::isPythonPathValid(const QString &path)
{ QFileInfo pythonInterpretatorInfo { path };
    if (!pythonInterpretatorInfo.exists() ||
        !pythonInterpretatorInfo.isFile() ||
        !pythonInterpretatorInfo.isExecutable()) {
        return false;
    }
    return true;

}
