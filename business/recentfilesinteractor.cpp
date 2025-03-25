#include "recentfilesinteractor.h"

#include <qstring.h>
#include "business/recentfilesmanager.h"

// Needed for unit tests
RecentFilesInteractor::RecentFilesInteractor(IRecentFilesManager *manager) {
    recentFilesManager = manager;
}


RecentFilesInteractor::RecentFilesInteractor() {
    recentFilesManager = new RecentFilesManager("com.whisperingwind", "ImageDiff");
}

RecentFilesInteractor::~RecentFilesInteractor() {
    if (recentFilesManager != nullptr) {
        delete recentFilesManager;
        recentFilesManager = nullptr;
    }
}


// Open images from the recent files menu.
// The menu item is formatted as "path to file 1 -> path to file 2".
std::optional<QStringPair> RecentFilesInteractor::getRecentFilesPathsByRecentMenuRecord(
                                                                    const QString& recentFileMenuRecord
                                                                        )
{
    auto pair = stringToPair(recentFileMenuRecord);
    if (!pair) {
        throw std::runtime_error("Error: Unable to load images!");
    }
    return pair;
}

QStringList RecentFilesInteractor::getRecentFilesMenuRecords() {
    QStringList result;
    auto pairs = recentFilesManager->getAllPairs();

    foreach (auto pair, pairs) {
        auto record = pairToString(pair);
        if (record.isEmpty()) {
            continue;
        }
        result.append(record);
    }

    return result;
}

void RecentFilesInteractor::addRecentFilesRecord(const QString &file1, const QString &file2) {
    recentFilesManager->addPair(file1, file2);
}

void RecentFilesInteractor::clear() {
    recentFilesManager->clear();
}

// Converts a QPair<QString, QString> to a formatted QString
QString RecentFilesInteractor::pairToString(const QStringPair& pair) {
    if (pair.first.isEmpty() || pair.second.isEmpty()) {
        return QString();
    }
    return pair.first + " -> " + pair.second;
}

// Converts a formatted QString back to a QPair<QString, QString>
std::optional<QStringPair> RecentFilesInteractor::stringToPair(const QString& str) {
    if (str.isEmpty()) {
        return std::nullopt;
    }
    return splitString(str, " -> ");
}

// Helper method to split a string with a specific separator
std::optional<QStringPair> RecentFilesInteractor::splitString(const QString& str,
                                                              const QString& separator
                                                              )
{
    int separatorIndex = str.indexOf(separator);

    if (separatorIndex == -1 || separatorIndex == 0 || separatorIndex == str.length() - separator.length()) {
        return std::nullopt;
    }

    QString firstPart = str.left(separatorIndex);
    QString secondPart = str.mid(separatorIndex + separator.length());

    if (firstPart.isEmpty() || secondPart.isEmpty()) {
        return std::nullopt;
    }

    return QStringPair(firstPart, secondPart);
}
