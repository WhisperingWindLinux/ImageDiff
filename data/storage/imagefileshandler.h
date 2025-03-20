#ifndef IMAGEFILESHANDLER_H
#define IMAGEFILESHANDLER_H

#include <qstring.h>
#include <qurl.h>
#include <domain/valueobjects/savefileinfo.h>
#include <data/storage/repositories/imagesrepository.h>

class IMainWindowCallbacks;
class RecentFilesManager;

class ImageFilesHandler
{
public:
    ImageFilesHandler(IMainWindowCallbacks *callbacks);

    QStringList getRecentFiles();
    void openImagesFromRecentMenu(QString recentFileMenuRecord, bool isUpdateRecentMenu);
    void openImagesFromDragAndDrop(QList<QUrl> urls);
    void getPathsFromUserAndOpenImages();
    void openImages(QString &Image1Path, QString &Image2Path, bool removeImageFilesAtExit, bool isUpdateRecentMenu);
    bool validateFile(const QString &filePath);
    void saveImage(SaveImageInfo info);

private:
    IMainWindowCallbacks *callbacks;
    std::unique_ptr<RecentFilesManager> recentFilesManager;
    std::unique_ptr<ImagesRepository> imageRepository;
    bool cleanUpImageFilesAtExit;
};

#endif // IMAGEFILESHANDLER_H
