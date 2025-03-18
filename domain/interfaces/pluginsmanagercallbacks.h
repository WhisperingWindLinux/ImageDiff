#ifndef PLUGINSMANAGERCALLBACKS_H
#define PLUGINSMANAGERCALLBACKS_H

#include <domain/interfaces/imageprocessor.h>

class IPluginsManagerCallbacks {
public:
    virtual void onPluginsLoaded(QList<shared_ptr<IImageProcessor> > processors)= 0;
};

#endif // PLUGINSMANAGERCALLBACKS_H
