#include "pluginsmanager.h"
#include <business/plugins/imageprocessordeserializer.h>
#include <domain/valueobjects/pyscriptinfo.h>
#include <business/pluginsettingsinteractor.h>

QList<std::shared_ptr<IImageProcessor> > PluginsManager::loadPlugins() {

    QList<std::shared_ptr<IImageProcessor> > result;

    PluginsSettingsInteractor pluginsSettingsInteractor;
    auto pluginSettings = pluginsSettingsInteractor.getPluginSettings();

    QDir dir(pluginSettings.pluginsDirectoryPath);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << pluginSettings.pluginsDirectoryPath;
        return result;
    }

    QList<PyScriptInfo> pyScriptsInfo;
    QStringList jsonFileNames = dir.entryList(QStringList() << "*.json", QDir::Files);

    foreach (auto jsonFileName, jsonFileNames) {
        QString jsonFilePath = dir.filePath(jsonFileName);
        QString pyScriptPath = dir.filePath(QFileInfo(jsonFileName).baseName() + ".py");
        auto pyScriptInfo = QFileInfo(pyScriptPath);
        if (!pyScriptInfo.exists() ||
            !pyScriptInfo.isFile() ||
            !pyScriptInfo.isReadable()
            )
        {
            continue;
        }
        PyScriptInfo info { jsonFilePath, pyScriptPath } ;
        pyScriptsInfo.append(info);
    }

    ImageProcessorDeserializer deserializer { pyScriptsInfo };
    auto processors = deserializer.deserialize();
    return processors;
}
