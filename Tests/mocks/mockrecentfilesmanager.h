#ifndef MOCKRECENTFILESMANAGER_H
#define MOCKRECENTFILESMANAGER_H

#include "business/recentfilesinteractor.h"
#include <domain/interfaces/business/irecentfilesmanager.h>


class MockRecentFilesManager : public IRecentFilesManager
{
public:
    MockRecentFilesManager();

    // IRecentFilesManager interface

    void addPair(const QString &file1, const QString &file2) override;
    void clear() override;
    QList<QStringPair> getAllPairs() const override;

private:
    QList<QStringPair> recentFileRecords;

};

#endif // MOCKRECENTFILESMANAGER_H
