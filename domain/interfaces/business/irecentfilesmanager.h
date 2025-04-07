#ifndef IRECENTFILESMANAGER_H
#define IRECENTFILESMANAGER_H

#include <qstring.h>
#include <domain/valueobjects/recentfilesrecord.h>

class IRecentFilesManager {
public:
    virtual ~IRecentFilesManager() = default;

    virtual void addRecord(const QString &filePath) = 0;
    virtual void addRecord(const QString &firstFilePath, const QString &secondFilePath) = 0;
    virtual void clear() = 0;
    virtual QList<RecentFilesRecord> getAllRecords() const = 0;
};

#endif // IRECENTFILESMANAGER_H
