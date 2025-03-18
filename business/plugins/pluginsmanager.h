#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <domain/interfaces/imageprocessor.h>
#include <domain/interfaces/pluginsmanagercallbacks.h>
#include <qlist.h>
#include <qobject.h>
#include <qthread.h>

using namespace std;

class PluginsManager : public QThread
{
    Q_OBJECT

signals:
    void resultReady(const QList<shared_ptr<IImageProcessor> > &processors);

public:
    PluginsManager(IPluginsManagerCallbacks *callbacks);
    void loadAsync();

private:
    IPluginsManagerCallbacks *callbacks;

private slots:
    void onPluginsLoaded(const QList<shared_ptr<IImageProcessor> > &processors);

    // QThread interface
protected:
    void run();
};

#endif // PLUGINMANAGER_H
