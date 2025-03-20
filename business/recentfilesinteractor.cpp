#include "recentfilesinteractor.h"

#include <qstring.h>
#include <business/recentfilesmanager.h>


RecentFilesInteractor::RecentFilesInteractor() {
    recentFilesManager = std::make_unique<RecentFilesManager>("com.whisperingwind", "ImageDiff");
}


// Open images from the recent files menu.
// The menu item is formatted as "path to file 1 -> path to file 2".
std::optional<QPair<QString, QString>> RecentFilesInteractor::getRecentFilePathsByRecentMenuRecord(
                                                                        QString& recentFileMenuRecord
                                                                        )
{
    auto pair = stringToPair(recentFileMenuRecord);
    if (!pair) {
        throw std::runtime_error("Error: Unable to load images!");
    }
    return pair;
}

QStringList RecentFilesInteractor::getRecentFileMenuRecords() {
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

// Converts a QPair<QString, QString> to a formatted QString
QString RecentFilesInteractor::pairToString(const QPair<QString, QString>& pair) {
    if (pair.first.isEmpty() || pair.second.isEmpty()) {
        return QString();
    }
    return pair.first + " -> " + pair.second;
}

// Converts a formatted QString back to a QPair<QString, QString>
std::optional<QPair<QString, QString>> RecentFilesInteractor::stringToPair(const QString& str) {
    if (str.isEmpty()) {
        return std::nullopt;
    }
    return splitString(str, " -> ");
}

// Helper method to split a string with a specific separator
std::optional<QPair<QString, QString>> RecentFilesInteractor::splitString(const QString& str,
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

    return QPair<QString, QString>(firstPart, secondPart);
}
