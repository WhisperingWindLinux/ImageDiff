#include "pluginmanager.h"


PluginManager::PluginManager() {}

void PluginManager::loadPlugins() {

    foreach (auto pluginDirPath, m_pluginDirectories) {

        QDir pluginsDir(pluginDirPath);

        foreach (auto fileName, pluginsDir.entryList(QDir::Files)) {
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            QObject *pluginInstance = loader.instance();

            if (!pluginInstance) {
                qWarning() << "Failed to load plugin:" << loader.errorString();
                continue;
            }

            IPluginInterface *plugin = qobject_cast<IPluginInterface *>(pluginInstance);
            if (plugin) {
                qDebug() << "Loaded plugin:" << plugin->pluginName();
                m_plugins.append(plugin);
            } else {
                qWarning() << "Invalid plugin interface in" << fileName;
            }
        }
    }
}

QList<IPluginInterface *> PluginManager::plugins() const {
    return m_plugins;
}

QStringList PluginManager::getPluginDirectories() {
    QStringList pluginDirs;

    QDir appDir(QCoreApplication::applicationDirPath());
#if defined(Q_OS_MAC)

    // On macOS, plugins are typically stored in "Contents/PlugIns" inside the .app bundle
    appDir.cdUp();
    appDir.cd("PlugIns"); // Move from "Contents/MacOS" to "Contents"
    pluginDirs << appDir.absolutePath();

    // User dir (~/Library/Application Support/MyApp/Plugins)
    QString userPluginDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Plugins";
    pluginDirs << userPluginDir;

    // Global dir (/Library/Application Support/MyApp/Plugins)
    QString globalPluginDir = "/Library/Application Support/ImageDiff/Plugins";
    pluginDirs << globalPluginDir;

#elif defined(Q_OS_WIN)
    // On Windows, plugins are typically stored in the "plugins" subdirectory next to the .exe
    QDir pluginsDir(appDir);
    pluginsDir.cd("plugins");
    return pluginsDir.absolutePath();
#elif defined(Q_OS_LINUX)
    // On Linux, plugins may be located in "/usr/lib/myapp/plugins" or next to the executable
    QDir pluginsDir(appDir);
    pluginsDir.cd("plugins");
    return pluginsDir.absolutePath();
#else
    // For other operating systems, use a directory next to the executable
    return QDir(appDir).absoluteFilePath("plugins");
#endif

    return pluginDirs;
}
