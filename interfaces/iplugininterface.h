#ifndef IPLUGININTERFACE_H
#define IPLUGININTERFACE_H

#include "interfaces/imageprocessor.h"
#include <qstring.h>
#include <QtPlugin>

// Plugin interface for all plugins
class IPluginInterface {
public:
    virtual ~IPluginInterface() {}

    // Returns the name of the plugin
    virtual QString pluginName() const = 0;

    // Returns the version of the plugin
    virtual int pluginVersion() const = 0;

    // Returns the type of the plugin (e.g., Filter or Comparator)
    virtual ImageProcessorType pluginType() const = 0;

    // Returns a description of the plugin
    virtual QString pluginDescription() const = 0;

    // Factory method to create an instance of the plugin's processor (e.g., IFilter or IComparator)
    virtual shared_ptr<IImageProcessor> createProcessor() const = 0;
};

#define PluginInterface_iid "com.ImageDiff.PluginInterface"
Q_DECLARE_INTERFACE(IPluginInterface, PluginInterface_iid)


#endif // IPLUGININTERFACE_H
