#ifndef IMAGEFILESINTERACTORS_H
#define IMAGEFILESINTERACTORS_H

#include "otherappinstancesinteractor.h"

#include <qstring.h>
#include <qurl.h>

#include <domain/valueobjects/savefileinfo.h>
#include <domain/interfaces/imagefilesinteractorlistener.h>
#include <domain/valueobjects/images.h>
#include <business/validation/imagevalidationrules.h>


class RecentFilesInteractor;
class ImageFilesHandler;

class ImageFilesInteractor
{
public:
    ImageFilesInteractor();
    ~ImageFilesInteractor();

    void openImages(const QString &image1Path, const QString &image2Path);
    void openImagesFromRecentMenu(const QString &recentFileMenuRecord);
    void openImagesFromDragAndDrop(const QList<QUrl> &urls);
    void openImagesViaCommandLine(const QString &image1Path, const QString &image2Path);
    void openImagesViaOpenFilesDialog();
    void openImagesFromVideos();
    void saveImage(const SaveImageInfo &info);
    void saveImageAs(const SaveImageInfo &info);
    
    bool subscribe(IImageFilesInteractorListener *listener);
    bool unsubscribe(const IImageFilesInteractorListener *listener);

    void cleanup();

private:
    ImagesPtr images;
    ImageFilesHandler *imageFileHandler;
    RecentFilesInteractor *recentFilesInteractor;
    QList<IImageFilesInteractorListener*> listeners;

    void validateImages(ImagesPtr images);

    std::optional<QString> savePixmapToTempDir(const QPixmap &pixmap, const QString &fileName);

    void notifyImagesOpened(const ImagesPtr images);
    void notifyImagesOpenFailed(const QString &error);
    void notifyImagesClosed();
    void notifySavingFileFailed(const QString &path);
    void notifyFileSavedSuccessfully(const QString &path);
};

#endif // IMAGEFILESINTERACTORS_H
