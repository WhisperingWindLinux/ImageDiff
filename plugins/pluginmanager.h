#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <qcoreapplication.h>
#include <qstandardpaths.h>

#include <interfaces/iplugininterface.h>

class PluginManager {
public:
    PluginManager();
    void loadPlugins();
    QList<IPluginInterface *> plugins() const;

private:
    QList<QString> m_pluginDirectories;
    QList<IPluginInterface *> m_plugins;

    QStringList getPluginDirectories();

};

#endif // PLUGINMANAGER_H
