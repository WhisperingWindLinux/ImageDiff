#include "pluginsmanager.h"
#include <business/plugins/imageprocessordeserializer.h>

#include <QThread>

#include <domain/valueobjects/pyscriptinfo.h>

#include <business/pluginsettingsinteractor.h>

PluginsManager::PluginsManager(IPluginsManagerCallbacks *callbacks)
    : callbacks(callbacks)
{
    connect(this, &PluginsManager::resultReady, &PluginsManager::onPluginsLoaded);
}

void PluginsManager::loadAsync() {
    start(QThread::Priority::TimeCriticalPriority);
}

void PluginsManager::onPluginsLoaded(const QList<shared_ptr<IImageProcessor> > &processors) {
    if (callbacks != nullptr) {
        callbacks->onPluginsLoaded(processors);
    }
}

void PluginsManager::run() {

    PluginSettingsInteractor pluginsSettingsInteractor;
    auto pluginSettings = pluginsSettingsInteractor.getPluginSettings();

    QDir dir(pluginSettings.pluginsDirectoryPath);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << pluginSettings.pluginsDirectoryPath;
        return;
    }

    QList<PyScriptInfo> pyScriptsInfo;
    QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);

    foreach (auto fileName, jsonFiles) {
        QFile file(dir.filePath(fileName));
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open file:" << file.fileName();
            continue;
        }

        QString pyScriptPath = dir.filePath(QFileInfo(fileName).baseName() + ".py");
        auto pyScriptInfo = QFileInfo(pyScriptPath);
        if (!pyScriptInfo.exists() ||
            !pyScriptInfo.isFile() ||
            !pyScriptInfo.isReadable()
            )
        {
            continue;
        }

        QByteArray jsonData = file.readAll();
        std::shared_ptr<QByteArray> jsonDataPtr = std::make_shared<QByteArray>(jsonData);
        if (jsonData.size() != 0) {
            pyScriptsInfo.append({ jsonDataPtr, pyScriptPath } );
        }
    }

    ImageProcessorDeserializer deserializer { pyScriptsInfo };
    auto processors = deserializer.deserialize();
    emit resultReady(processors);
}
