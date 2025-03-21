#ifndef IMAGEFILESINTERACTORS_H
#define IMAGEFILESINTERACTORS_H

#include <qstring.h>
#include <qurl.h>

#include <domain/valueobjects/savefileinfo.h>
#include <domain/interfaces/imagefilesinteractorlistener.h>
#include <domain/interfaces/mainwindowcallbacks.h>
#include <data/repositories/imagesrepository.h>


class IMainWindowCallbacks;
class RecentFilesInteractor;
class ImageFilesHandler;

class ImageFilesInteractors
{
public:
    ImageFilesInteractors(IMainWindowCallbacks *callbacks);
    ~ImageFilesInteractors();

    void openImagesFromRecentMenu(const QString &recentFileMenuRecord);
    void openImagesFromDragAndDrop(const QList<QUrl> &urls);
    void openImagesViaCommandLine(const QString &image1Path, const QString &image2Path);
    void openImagesViaOpenFilesDialog();
    void saveImage(const SaveImageInfo &info);

    bool subscribe(const IImageFilesInteractorListenerPtr listener);
    bool unsubscribe(const IImageFilesInteractorListenerPtr listener);

private:
    IMainWindowCallbacks *callbacks;
    ImageFilesHandler *imageFileHandler;
    RecentFilesInteractor *recentFilesInteractor;
    ImagesRepositoryPtr imagesRepository;
    QList<IImageFilesInteractorListenerPtr> listeners;

    void notifyComparedImagesUpdated(IImagesRepositoryPtr imageRepository);
    void notifyComparisonResultImageOpened(IImagesRepositoryPtr imageRepository);
    void notifyImagesClosed();
    void notifyComparedImagesOpened(IImagesRepositoryPtr imageRepository);
    void notifyComparedImagedsOpenFailed(const QString &error);
    void notifyComparisonResultImageOpenedFailed(const QString &error);
    void notifyComparedImagesOpenFailed(const QString &error);
    void notifySaveFileFailed(const QString &path);
    void notifyFileSavedSuccessfully(const QString &path);
};

#endif // IMAGEFILESINTERACTORS_H
