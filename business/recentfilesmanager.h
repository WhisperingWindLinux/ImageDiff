#ifndef RECENTFILESMANAGER_H
#define RECENTFILESMANAGER_H

#include <QString>
#include <QList>
#include <QPair>
#include <QSettings>

#include <domain/interfaces/business/irecentfilesmanager.h>

// Manages a list of recently opened files. The application opens two files at a time,
// so the record of opened files is stored in the format '/path/to/file1.png -> /path/to/file2.png'.

class RecentFilesManager : public IRecentFilesManager {
public:
    RecentFilesManager(const QString &organization, const QString &application, int maxPairs = 8);
    virtual ~RecentFilesManager() = default;

    // IRecentFilesManager interface

    void addRecord(const QString &firstFilePath, const QString &secondFilePath) override;
    void addRecord(const QString &filePath) override;
    QList<RecentFilesRecord> getAllRecords() const override;
    void clear() override;

private:
    static const QString SETTINGS_KEY;  // Key used to store recent files in QSettings

    // A list of recent file pairs, such as '/path/to/file1.png -> /path/to/file2.png'
    QList<RecentFilesRecord> mRecentFilesRecords;

    int mMaxRecentPairs;   // Maximum number of recent file pairs
    QSettings mSettings;   // QSettings instance for persistent storage

    void saveToSettings();
    void loadFromSettings();
    void trimExcessPairs();    // Removes excess file pairs if the list exceeds the maximum allowed size
};

#endif // RECENTFILESMANAGER_
