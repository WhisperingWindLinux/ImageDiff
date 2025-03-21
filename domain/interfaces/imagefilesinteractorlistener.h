#ifndef IMAGEFILESINTERACTORLISTENER_H
#define IMAGEFILESINTERACTORLISTENER_H

#include <domain/valueobjects/images.h>

class IImageFilesInteractorListener {
public:
    virtual void onImagesOpened(const ImagesPtr images) = 0;
    virtual void onImagesOpenFailed(const QString &error) = 0;
    virtual void onImagesClosed() = 0;
    virtual void onSavingFileFailed(const QString &path) = 0;
    virtual void onFileSavedSuccessfully(const QString &path) = 0;
};

#endif // IMAGEFILESINTERACTORLISTENER_H
