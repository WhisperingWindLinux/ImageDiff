#ifndef IMAGEFILESINTERACTORLISTENER_H
#define IMAGEFILESINTERACTORLISTENER_H

#include <domain/interfaces/imagerepository.h>

class IImageFilesInteractorListener {
public:
    bool onComparedImagesOpened(const IImagesRepositoryPtr imageRepository);
    void onImagesClosed();
    bool onComparisonResultImageOpened(const IImagesRepositoryPtr imageRepository);
    void onComparedImagesUpdated(const IImagesRepositoryPtr imageRepository);
    void onComparedImagesOpenFailed(const QString &error);
    void onComparisonResultImageOpenedFailed(const QString &error);
    void onComparedImagedsOpenFailed(const QString &error);
    void onSaveFileFailed(const QString &path);
    void onFileSavedSuccessfully(const QString &path);
};

typedef std::shared_ptr<IImageFilesInteractorListener> IImageFilesInteractorListenerPtr;

#endif // IMAGEFILESINTERACTORLISTENER_H
