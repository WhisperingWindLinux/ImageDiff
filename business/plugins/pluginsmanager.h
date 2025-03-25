#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <domain/interfaces/business/imageprocessor.h>


class IPluginsManagerCallbacks;

class PluginsManager
{
public:
    PluginsManager() = default;
    ~PluginsManager() = default;

    QList<std::shared_ptr<IImageProcessor> > loadPlugins();
};

#endif // PLUGINMANAGER_H
