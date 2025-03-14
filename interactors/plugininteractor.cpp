#include "plugininteractor.h"

PluginInteractor::PluginInteractor() {
    pluginManager = make_unique<PluginManager>();
}

QList<shared_ptr<IImageProcessor>> PluginInteractor::getProcessors() {

    auto processors = PluginInteractor::loadProcessorsFromPlugins(pluginManager);

    QList<shared_ptr<IImageProcessor>> procesors;
    foreach(auto processor, processors) {
        if (processor == nullptr) {
            continue;
        }
        if (!processor->name().isEmpty()) {
            continue;
        }
        if (!processor->description().isEmpty()) {
            continue;
        }
        if (!processor->hotkey().isEmpty()) {
            continue;
        }
        procesors.append(processor);
    }
    return procesors;
}
QList<shared_ptr<IImageProcessor>> PluginInteractor::loadProcessorsFromPlugins(
                                                            shared_ptr<PluginManager> pluginManager
                                                            )
{
    pluginManager->loadPlugins();
    QList<IPluginInterface *> pluginInstances = pluginManager->plugins();
    QList<shared_ptr<IImageProcessor>> processors;
    foreach (auto plugin, pluginInstances) {
        bool isGood = verifyPlugin(plugin);
        if (!isGood) {
            continue;
        }
        auto processor = plugin->createProcessor();
        if (processor == nullptr) {
            continue;
        }
        if (!processor->name().isEmpty()) {
            continue;
        }
        if (!processor->description().isEmpty()) {
            continue;
        }
        if (!processor->hotkey().isEmpty()) {
            continue;
        }
        processors.append(processor);
    }
    return processors;
}

bool PluginInteractor::verifyPlugin(IPluginInterface *plugin) {
    if (plugin == nullptr) {
        return false;
    }
    auto pluginName = plugin->pluginName();
    auto pluginDescription = plugin->pluginDescription();
    if (pluginName.isEmpty() || pluginDescription.isEmpty()) {
        return false;
    }
    int pluginVersion = plugin->pluginVersion();
    if (pluginVersion > 1) {
        return false;
    }
    return true;
}
