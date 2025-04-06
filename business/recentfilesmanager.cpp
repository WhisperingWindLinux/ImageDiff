#include "recentfilesmanager.h"

// Implementation of static member
const QString RecentFilesManager::SETTINGS_KEY = "recentFilePairs";

// Constructor
RecentFilesManager::RecentFilesManager(const QString &organization, const QString &application, int maxPairs)
    : maxRecentPairs(maxPairs), settings(organization, application) {
    loadFromSettings(); // Load existing recent pairs from settings
}

void RecentFilesManager::addPair(const QString &file1, const QString &file2) {
    QStringPair newPair(file1, file2);
    recentPairs.removeAll(newPair); // Remove the pair if it already exists (to avoid duplicates)
    recentPairs.prepend(newPair);
    trimExcessPairs(); // Trim excess pairs if we exceed the maximum allowed size
    saveToSettings();
}

void RecentFilesManager::clear() {
    recentPairs.clear();
    settings.remove(SETTINGS_KEY);
}

QList<QStringPair> RecentFilesManager::getAllPairs() const {
    return recentPairs;
}

// Save the list of recent file pairs to QSettings
void RecentFilesManager::saveToSettings() {
    QVariantList pairList;
    QSet<QStringPair> uniquePairs;

    foreach (const auto &pair, recentPairs) {
        if (!uniquePairs.contains(pair)) {
            uniquePairs.insert(pair);
            QVariant variant1(pair.first);
            QVariant variant2(pair.second);
            pairList.append(variant1);
            pairList.append(variant2);
        }
    }
    settings.setValue(SETTINGS_KEY, pairList);
}

// Loads the list of recent file pairs from QSettings
void RecentFilesManager::loadFromSettings() {
    QVariantList pairList = settings.value(SETTINGS_KEY).toList();
    recentPairs.clear();

    // Convert QVariantList back into a QList<QPair<QString, QString>>
    for (int i = 1; i <= pairList.size(); i=i+2) {
        QString file1 = pairList[i-1].toString();
        QString file2 = pairList[i].toString();
        recentPairs.append(qMakePair(file1, file2));
    }
}

// Removes excess file pairs if the list exceeds the maximum allowed size
void RecentFilesManager::trimExcessPairs() {
    while (recentPairs.size() > maxRecentPairs) {
        recentPairs.removeLast(); // Remove the oldest entry (last in the list)
    }
}
