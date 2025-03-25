#include "mockrecentfilesmanager.h"

MockRecentFilesManager::MockRecentFilesManager() {}


void MockRecentFilesManager::addPair(const QString &file1, const QString &file2) {
    QStringPair pair { file1, file2 };
    recentFileRecords.removeAll(pair);
    recentFileRecords.push_front(pair);
}

void MockRecentFilesManager::clear() {
    recentFileRecords.clear();
}

QList<QStringPair> MockRecentFilesManager::getAllPairs() const {
    return recentFileRecords;
}
