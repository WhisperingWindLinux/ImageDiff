#include "recentfilesinteractor.h"

#include <qstring.h>
#include "business/recentfilesmanager.h"

// Needed for unit tests
RecentFilesInteractor::RecentFilesInteractor(IRecentFilesManager *manager) {
    mRecentFilesManager = manager;
}


RecentFilesInteractor::RecentFilesInteractor() {
    mRecentFilesManager = new RecentFilesManager("com.whisperingwind", "TwinPix");
}

RecentFilesInteractor::~RecentFilesInteractor() {
    if (mRecentFilesManager != nullptr) {
        delete mRecentFilesManager;
        mRecentFilesManager = nullptr;
    }
}


// Open images from the recent files menu.
// The menu item is formatted as "path to file 1 -> path to file 2" or "path to file 1".
std::optional<RecentFilesRecord> RecentFilesInteractor::getRecentFilesPathsByRecentMenuRecord(
                                                                const QString& recentFileMenuRecord
                                                                )
{
    auto record = stringToRecord(recentFileMenuRecord);
    if (!record) {
        throw std::runtime_error("Unable to open image(s).");
    }
    return record;
}

QStringList RecentFilesInteractor::getRecentFilesMenuRecords() {
    QStringList result;
    auto records = mRecentFilesManager->getAllRecords();

    foreach (auto record, records) {
        auto recordAsString = recordToString(record);
        if (!recordAsString) {
            continue;
        }
        result.append(recordAsString.value());
    }
    return result;
}

void RecentFilesInteractor::addRecentFilesRecord(const QString &firstPath, const QString &secondPath) {
    mRecentFilesManager->addRecord(firstPath, secondPath);
}

void RecentFilesInteractor::addRecentFilesRecord(const QString &firstPath) {
    mRecentFilesManager->addRecord(firstPath);
}

void RecentFilesInteractor::clear() {
    mRecentFilesManager->clear();
}

// Converts RecentFilesRecord to a formatted QString
std::optional<QString> RecentFilesInteractor::recordToString(const RecentFilesRecord &record) {
    if (record.getFirstPath().isEmpty()) {
        return std::nullopt;
    }
    if (record.isPairPathsRecord() && record.getSecondPath().isEmpty()) {
        return std::nullopt;
    }
    if (record.isPairPathsRecord()) {
        return QString("%1 -> %2")
            .arg(record.getFirstPath())
            .arg(record.getSecondPath());
    } else {
        return record.getFirstPath();
    }
}

// Converts a formatted QString back to RecentFilesRecord
std::optional<RecentFilesRecord> RecentFilesInteractor::stringToRecord(const QString& str) {
    if (str.isEmpty()) {
        return std::nullopt;
    }
    QString splitter = " -> ";
    if (str.contains(splitter)) {
        return splitString(str, splitter);
    } else {
        return RecentFilesRecord{str};
    }
}

// Helper method to split a string with a specific separator
std::optional<RecentFilesRecord> RecentFilesInteractor::splitString(const QString& str,
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

    return RecentFilesRecord{firstPart, secondPart};
}
