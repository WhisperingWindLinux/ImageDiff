#include <business/recentfilesmanager.h>

#include "recentmenuintercator.h"

#include <qstring.h>

RecentFilesRepository::RecentFilesRepository() {
    recentFilesManager = std::make_unique<RecentFilesManager>("com.whisperingwind", "ImageDiff");

}


// Open images from the recent files menu.
// The menu item is formatted as "path to file 1 -> path to file 2".
void RecentFilesRepository::openImagesFromRecentMenu(QString& recentFileMenuRecord,
                                                    bool isUpdateRecentMenu
                                                    )
{
    auto formatter = make_unique<RecentFilesPresenter>();
    std::optional<QPair<QString, QString>> pair = formatter->stringToPair(recentFileMenuRecord);
    if (!pair) {
        throw std::runtime_error("Error: Unable to load images!");
    }
    openImages(pair->first, pair->second, false, isUpdateRecentMenu);
}


QStringList ImageFilesHandler::getRecentFiles() {

    QStringList result;
    auto formatter = make_unique<RecentFilesPresenter>();
    auto pairs = recentFilesManager->getAllPairs();

    foreach (auto pair, pairs) {
        auto record = formatter->pairToString(pair);
        if (record.isEmpty()) {
            continue;
        }
        result.append(record);
    }

    return result;
}
