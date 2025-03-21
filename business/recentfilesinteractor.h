#ifndef RECENTFILESINTERACTOR_H
#define RECENTFILESINTERACTOR_H

#include <business/recentfilesmanager.h>

class RecentFilesInteractor
{
public:
    RecentFilesInteractor();
    
    std::optional<QPair<QString, QString> > getRecentFilesPathsByRecentMenuRecord(const QString &recentFileMenuRecord);

    QStringList getRecentFilesMenuRecords();

    void addRecentFilesRecord(const QString &file1, const QString &file2);

    void clear();

private:
    std::unique_ptr<RecentFilesManager> recentFilesManager;

    QString pairToString(const QPair<QString, QString> &pair);
    std::optional<QPair<QString, QString> > splitString(const QString &str, const QString &separator);
    std::optional<QPair<QString, QString> > stringToPair(const QString &str);
};

#endif // RECENTFILESINTERACTOR_H
