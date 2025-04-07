#ifndef PLUGINSREPOSITORY_H
#define PLUGINSREPOSITORY_H

#include <QString>
#include <QSettings>

#include <domain/valueobjects/pluginssettings.h>

using namespace std;

class PluginsSettingsRepository {
public:
    PluginsSettingsRepository();

    PluginsSettings getSettings();

    void upadteSettings(const PluginsSettings &pluginsSettings);

private:
    unique_ptr<QSettings> mSettings;
};

#endif // PLUGINSREPOSITORY_H


