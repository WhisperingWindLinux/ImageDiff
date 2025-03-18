#ifndef PLUGINSSETTINGS_H
#define PLUGINSSETTINGS_H

#include <qstring.h>

struct PluginsSettings {

  PluginsSettings(QString pythonInterpreterPath, QString pluginsDirectoryPath)
      : pythonInterpreterPath(std::move(pythonInterpreterPath)),
        pluginsDirectoryPath(std::move(pluginsDirectoryPath)) {}

  const QString pythonInterpreterPath;
  const QString pluginsDirectoryPath;
};

#endif // PLUGINSSETTINGS_H
