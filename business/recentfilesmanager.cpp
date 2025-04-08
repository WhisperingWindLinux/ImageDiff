#include "recentfilesmanager.h"

// Implementation of static member
const QString RecentFilesManager::SETTINGS_KEY = "recentFilePairs";

// Constructor
RecentFilesManager::RecentFilesManager(const QString &organization,
                                       const QString &application,
                                       int maxPairs
                                       )
    : mMaxRecentPairs(maxPairs),
    mSettings(organization, application)
{
    loadFromSettings(); // Load existing recent pairs from settings
}

void RecentFilesManager::addRecord(const QString &firstFilePath, const QString &secondFilePath) {
    RecentFilesRecord newRecord(firstFilePath, secondFilePath);
    mRecentFilesRecords.removeAll(newRecord); // Remove the pair if it already exists (to avoid duplicates)
    mRecentFilesRecords.prepend(newRecord);
    trimExcessPairs(); // Trim excess pairs if we exceed the maximum allowed size
    saveToSettings();
}

void RecentFilesManager::addRecord(const QString &filePath) {
    RecentFilesRecord newRecord{filePath};
    mRecentFilesRecords.removeAll(newRecord); // Remove the pair if it already exists (to avoid duplicates)
    mRecentFilesRecords.prepend(newRecord);
    trimExcessPairs(); // Trim excess pairs if we exceed the maximum allowed size
    saveToSettings();
}

void RecentFilesManager::clear() {
    mRecentFilesRecords.clear();
    mSettings.remove(SETTINGS_KEY);
}

QList<RecentFilesRecord> RecentFilesManager::getAllRecords() const {
    return mRecentFilesRecords;
}

// Save the list of recent file pairs to QSettings
void RecentFilesManager::saveToSettings() {
    QVariantList recordsList;
    QSet<RecentFilesRecord> uniqueRecords;

    foreach (const auto &record, mRecentFilesRecords) {
        if (!uniqueRecords.contains(record)) {
            uniqueRecords.insert(record);
            QVariant variant1(record.getFirstPath());
            QVariant variant2(record.getSecondPath());
            recordsList.append(variant1);
            recordsList.append(variant2);
        }
    }
    mSettings.setValue(SETTINGS_KEY, recordsList);
}

// Loads the list of recent file pairs from QSettings
void RecentFilesManager::loadFromSettings() {
    QVariantList pairList = mSettings.value(SETTINGS_KEY).toList();
    mRecentFilesRecords.clear();

    // Convert QVariantList back into RecentFilesRecord
    for (int i = 1; i <= pairList.size(); i=i+2) {
        QString firstPath = pairList[i-1].toString();
        QString secondPath = pairList[i].toString();
        if (secondPath.isEmpty()) {
            mRecentFilesRecords.append(RecentFilesRecord{firstPath});
        } else {
            mRecentFilesRecords.append(RecentFilesRecord{firstPath, secondPath});
        }
    }
}

// Removes excess file pairs if the list exceeds the maximum allowed size
void RecentFilesManager::trimExcessPairs() {
    while (mRecentFilesRecords.size() > mMaxRecentPairs) {
        mRecentFilesRecords.removeLast(); // Remove the oldest entry (last in the list)
    }
}
