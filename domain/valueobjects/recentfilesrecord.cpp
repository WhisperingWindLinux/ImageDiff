#include "recentfilesrecord.h"


RecentFilesRecord::RecentFilesRecord(const QString &path)
    : mIsPairPathsRecord(false),
    mFirstPath(path)
{
}

RecentFilesRecord::RecentFilesRecord(const QString &firstPath, const QString &secondPath)
    : mIsPairPathsRecord(true),
    mFirstPath(firstPath),
    mSecondPath(secondPath)
{
}

bool RecentFilesRecord::isPairPathsRecord() const {
    return mIsPairPathsRecord;
}

QString RecentFilesRecord::getFirstPath() const {
    return mFirstPath;
}

QString RecentFilesRecord::getSecondPath() const {
    return mSecondPath;
}

bool RecentFilesRecord::operator==(const RecentFilesRecord &other) const {
    return (mFirstPath == other.mFirstPath) &&
           (mSecondPath == other.mSecondPath);
}
