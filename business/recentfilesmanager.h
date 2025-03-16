#ifndef RECENTFILESMANAGER_H
#define RECENTFILESMANAGER_H

#include <QString>
#include <QList>
#include <QPair>
#include <QSettings>

class RecentFilesManager {
public:
    // Constructor
    RecentFilesManager(const QString &organization, const QString &application, int maxPairs = 5);

    // Adds a pair of files to the recent list
    void addPair(const QString &file1, const QString &file2);

    // Clears all recent file pairs
    void clear();

    // Retrieves all recent file pairs
    QList<QPair<QString, QString>> getAllPairs() const;

    friend class RecentFileManagerTest;

private:
    // Saves the current list of recent file pairs to QSettings
    void saveToSettings();

    // Loads the list of recent file pairs from QSettings
    void loadFromSettings();

    // Removes excess file pairs if the list exceeds the maximum allowed size
    void trimExcessPairs();

private:
    QList<QPair<QString, QString>> recentPairs;  // List of recent file pairs
    int maxRecentPairs;                          // Maximum number of recent file pairs
    QSettings settings;                          // QSettings instance for persistent storage

    static const QString SETTINGS_KEY;           // Key used to store recent files in QSettings
};

#endif // RECENTFILESMANAGER_H
