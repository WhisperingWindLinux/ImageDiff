#ifndef PLUGININTERACTOR_H
#define PLUGININTERACTOR_H

#include <plugins/pluginmanager.h>


class PluginInteractor
{
public:
    PluginInteractor();
    ~PluginInteractor() = default;

    QList<shared_ptr<IImageProcessor> > getProcessors();

private:
    shared_ptr<PluginManager> pluginManager;
    QList<shared_ptr<IImageProcessor>> processors;

    static QList<shared_ptr<IImageProcessor>> loadProcessorsFromPlugins(
                                            shared_ptr<PluginManager> pluginManager
                                            );

    static bool verifyPlugin(IPluginInterface *plugin);

};

#endif // PLUGININTERACTOR_H
