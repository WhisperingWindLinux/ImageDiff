#ifndef RECENTFILESINTERACTOR_H
#define RECENTFILESINTERACTOR_H

#include "business/recentfilesmanager.h"

class IRecentFilesManager;

class RecentFilesInteractor
{
public:   
    RecentFilesInteractor();
    RecentFilesInteractor(IRecentFilesManager *manager); // Needed for unit tests
    virtual ~RecentFilesInteractor();
    
    std::optional<RecentFilesRecord> getRecentFilesPathsByRecentMenuRecord(const QString &recentFileMenuRecord);

    QStringList getRecentFilesMenuRecords();

    void addRecentFilesRecord(const QString &firstPath, const QString &secondPath);
    void addRecentFilesRecord(const QString &firstPath);

    void clear();

private:
    IRecentFilesManager *mRecentFilesManager;

    std::optional<QString> recordToString(const RecentFilesRecord &record);
    std::optional<RecentFilesRecord> splitString(const QString &str, const QString &separator);
    std::optional<RecentFilesRecord> stringToRecord(const QString &str);
};

#endif // RECENTFILESINTERACTOR_H
