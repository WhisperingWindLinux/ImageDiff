#include "pluginsettingsinteractor.h"

#include <qfileinfo.h>

#include <business/imageanalysis/imageprocessorsmanager.h>

PluginsSettingsInteractor::PluginsSettingsInteractor() {
    pluginSettingsRepository = make_unique<PluginsSettingsRepository>();
}

bool PluginsSettingsInteractor::updatePluginSettings(PluginsSettings pluginSettings, QString& error) {
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

PluginsSettings PluginsSettingsInteractor::getPluginSettings() {
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

bool PluginsSettingsInteractor::isPluginDirPathValid(const QString &path) {
    if(path.isEmpty()) {
        return true;
    }
    QFileInfo pluginsDirInfo { path };
    if (!pluginsDirInfo.exists() ||
        !pluginsDirInfo.isDir()) {
        return false;
    }
    return true;
}

bool PluginsSettingsInteractor::isPythonPathValid(const QString &path) {
    if(path.isEmpty()) {
        return true;
    }
    QFileInfo pythonInterpretatorInfo { path };
    if (!pythonInterpretatorInfo.exists() ||
        !pythonInterpretatorInfo.isFile() ||
        !pythonInterpretatorInfo.isExecutable()) {
        return false;
    }
    return true;

}
