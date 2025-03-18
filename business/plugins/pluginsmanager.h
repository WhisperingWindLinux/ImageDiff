#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <domain/interfaces/imageprocessor.h>


class IPluginsManagerCallbacks;

class PluginsManager
{
public:
    PluginsManager();
    QList<std::shared_ptr<IImageProcessor> > loadPlugins();
};

#endif // PLUGINMANAGER_H
