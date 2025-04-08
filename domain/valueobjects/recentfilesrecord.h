#ifndef RECENTFILESRECORD_H
#define RECENTFILESRECORD_H

#include <qstring.h>


class RecentFilesRecord
{
public:
    RecentFilesRecord(const QString &path);
    RecentFilesRecord(const QString &firstPath, const QString &secondPath);

    bool isPairPathsRecord() const;
    QString getFirstPath() const;
    QString getSecondPath() const;

    bool operator==(const RecentFilesRecord &other) const;

private:
    bool mIsPairPathsRecord;
    QString mFirstPath;
    QString mSecondPath;
};

inline size_t qHash(const RecentFilesRecord &record, size_t seed) {
    uint hash1 = qHash(record.getFirstPath(), seed);
    uint hash2 = qHash(record.getSecondPath(), seed);
    return hash1 ^ hash2;
}

#endif // RECENTFILESRECORD_H
