#ifndef IMAGEFILESINTERACTORS_H
#define IMAGEFILESINTERACTORS_H

#include <qstring.h>
#include <qurl.h>
#include <domain/valueobjects/savefileinfo.h>
#include <domain/interfaces/presentation/imagefilesinteractorlistener.h>
#include <domain/valueobjects/images.h>
#include <business/validation/imagevalidationrules.h>

class RecentFilesInteractor;
class ImageFilesHandler;

class ImageFilesInteractor {
public:
    ImageFilesInteractor();
    ~ImageFilesInteractor();

    void openImages(const QString &firstImagePath, const QString &secondImagePath);
    void openImagesFromRecentMenu(const QString &recentFileMenuRecord);
    void openImagesFromDragAndDrop(const QList<QUrl> &urls);
    void openImagesViaCommandLine(const QString &firstImagePath, const QString &secondImagePath);
    void openImagesViaOpenFilesDialog();
    void openImageViaOpenFilesDialog();
    void openImagesFromVideos();
    void openImageFromClipboard();
    void saveImageAs(const SaveImageInfo &info);
    
    bool subscribe(IImageFilesInteractorListener *listener);
    bool unsubscribe(IImageFilesInteractorListener *listener);

    void cleanup();
    
private:
    ImageHolderPtr mImages;
    ImageFilesHandler *mImageFileHandler;
    RecentFilesInteractor *mRecentFilesInteractor;
    QList<IImageFilesInteractorListener*> mListeners;

    std::optional<QString> savePixmapToTempDir(const QPixmap &pixmap, const QString &fileName);
    void validateImages(ImageHolderPtr images);
    bool isFileInTempFolder(const QString &filePath);

    void notifyImagesOpened(const ImageHolderPtr images);
    void notifyImagesOpenFailed(const QString &error);
    void notifyImagesClosed();
    void notifySavingFileFailed(const QString &path);
    void notifyFileSavedSuccessfully(const QString &path);
};

#endif // IMAGEFILESINTERACTORS_H
