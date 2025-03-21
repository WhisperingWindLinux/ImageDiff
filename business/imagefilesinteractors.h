#ifndef IMAGEFILESINTERACTORS_H
#define IMAGEFILESINTERACTORS_H

#include <qstring.h>
#include <qurl.h>

#include <domain/valueobjects/savefileinfo.h>
#include <domain/interfaces/imagefilesinteractorlistener.h>
#include <data/repositories/imagesrepository.h>
#include <domain/valueobjects/images.h>


class RecentFilesInteractor;
class ImageFilesHandler;

class ImageFilesInteractors
{
public:
    ImageFilesInteractors();
    ~ImageFilesInteractors();

    void openImagesFromRecentMenu(const QString &recentFileMenuRecord);
    void openImagesFromDragAndDrop(const QList<QUrl> &urls);
    void openImagesViaCommandLine(const QString &image1Path, const QString &image2Path);
    void openImagesViaOpenFilesDialog();
    void saveImage(const SaveImageInfo &info);

    bool subscribe(const IImageFilesInteractorListenerPtr listener);
    bool unsubscribe(const IImageFilesInteractorListenerPtr listener);

private:
    ImagesPtr images;
    ImageFilesHandler *imageFileHandler;
    RecentFilesInteractor *recentFilesInteractor;
    QList<IImageFilesInteractorListenerPtr> listeners;

    void notifyImagesOpened(const ImagesPtr images);
    void notifyImagesOpenFailed(const QString &error);
    void notifyImagesClosed();
    void notifySavingFileFailed(const QString &path);
    void notifyFileSavedSuccessfully(const QString &path);
};

#endif // IMAGEFILESINTERACTORS_H
