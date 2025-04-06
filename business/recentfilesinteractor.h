#ifndef RECENTFILESINTERACTOR_H
#define RECENTFILESINTERACTOR_H

#include "business/recentfilesmanager.h"

class IRecentFilesManager;

class RecentFilesInteractor
{
public:   
    RecentFilesInteractor();
    virtual ~RecentFilesInteractor();

    RecentFilesInteractor(IRecentFilesManager *manager); // Needed for unit tests
    
    std::optional<QStringPair> getRecentFilesPathsByRecentMenuRecord(const QString &recentFileMenuRecord);

    QStringList getRecentFilesMenuRecords();

    void addRecentFilesRecord(const QString &file1, const QString &file2);

    void clear();

private:
    IRecentFilesManager *recentFilesManager;

    QString pairToString(const QStringPair &pair);
    std::optional<QStringPair> splitString(const QString &str, const QString &separator);
    std::optional<QStringPair> stringToPair(const QString &str);
};

#endif // RECENTFILESINTERACTOR_H
