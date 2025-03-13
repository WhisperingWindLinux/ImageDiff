#include "recentfilesmanager.h"

// Implementation of static member
const QString RecentFilesManager::SETTINGS_KEY = "recentFilePairs";

// Constructor
RecentFilesManager::RecentFilesManager(const QString &organization, const QString &application, int maxPairs)
    : maxRecentPairs(maxPairs), settings(organization, application) {
    loadFromSettings(); // Load existing recent pairs from settings
}

// Adds a pair of files to the recent list
void RecentFilesManager::addPair(const QString &file1, const QString &file2) {
    QPair<QString, QString> newPair(file1, file2);

    // Remove the pair if it already exists (to avoid duplicates)
    recentPairs.removeAll(newPair);

    // Add the new pair to the beginning of the list
    recentPairs.prepend(newPair);

    // Trim excess pairs if we exceed the maximum allowed size
    trimExcessPairs();

    // Save the updated list to QSettings
    saveToSettings();
}

// Clears all recent file pairs
void RecentFilesManager::clear() {
    recentPairs.clear();       // Clear the in-memory list
    settings.remove(SETTINGS_KEY); // Remove from QSettings
}

// Retrieves all recent file pairs
QList<QPair<QString, QString>> RecentFilesManager::getAllPairs() const {
    return recentPairs;
}

// Saves the current list of recent file pairs to QSettings
void RecentFilesManager::saveToSettings() {
    QVariantList pairList;
    QSet<QPair<QString, QString>> uniquePairs;

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
