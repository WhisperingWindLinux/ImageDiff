#include "imagefilesinteractors.h"

#include "recentfilesinteractor.h"
#include <data/storage/imagefileshandler.h>
#include <data/repositories/imagesrepository.h>

ImageFilesInteractors::ImageFilesInteractors() {
    imageFileHandler = new ImageFilesHandler();
    recentFilesInteractor = new RecentFilesInteractor();
}

ImageFilesInteractors::~ImageFilesInteractors() {
    if (recentFilesInteractor != nullptr) {
        delete recentFilesInteractor;
        recentFilesInteractor = nullptr;
    }
    if (imageFileHandler != nullptr) {
        delete imageFileHandler;
        imageFileHandler = nullptr;
    }
}

void ImageFilesInteractors::openImagesFromRecentMenu(const QString &recentFileMenuRecord) {
    try {
        auto paths = recentFilesInteractor->getRecentFilePathsByRecentMenuRecord(recentFileMenuRecord);
        if (!paths) {
            throw std::runtime_error("Error: Unable to open images!");
        }
        images = imageFileHandler->openImages(paths->first, paths->second);
        if (images == nullptr) {
            throw std::runtime_error("Unknown error.");
        }
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        notifyImagesOpenFailed(e.what());
    }
}

void ImageFilesInteractors::openImagesFromDragAndDrop(const QList<QUrl> &urls) {
    try {
        images = imageFileHandler->openImages(urls);
        if (images == nullptr) {
            throw std::runtime_error("Unknown error.");
        }
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        notifyImagesOpenFailed(e.what());
    }
}

void ImageFilesInteractors::openImagesViaCommandLine(const QString &image1Path,
                                                     const QString &image2Path
                                                     )
{
    try {
        ImagesPtr images = imageFileHandler->openImages(image1Path, image2Path);
        if (images == nullptr) {
            throw std::runtime_error("Unknown error.");
        }
        images->markAsTemporary();
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        notifyImagesOpenFailed(e.what());
    }
}

void ImageFilesInteractors::openImagesViaOpenFilesDialog() {
    try {
        ImagesPtr images = imageFileHandler->openImages();
        if (images == nullptr) {
            throw std::runtime_error("Unknown error.");
        }
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        notifyImagesOpenFailed(e.what());
    }
}

void ImageFilesInteractors::saveImage(const SaveImageInfo &info) {
    FileSaveResult result = imageFileHandler->saveImage(info, images);
    if (result.isSaved) {
        notifyFileSavedSuccessfully(result.path);
    } else {
        notifySavingFileFailed(result.path);
    }
}

bool ImageFilesInteractors::subscribe(const IImageFilesInteractorListenerPtr listener) {
    if (listener == nullptr) {
        return false;
    }
    if (listeners.contains(listener)) {
        return false;
    }
    listeners.append(listener);
    return true;
}

bool ImageFilesInteractors::unsubscribe(const IImageFilesInteractorListenerPtr listener) {
    if (listener == nullptr) {
        return false;
    }
    return listeners.removeOne(listener);
}

void ImageFilesInteractors::notifyImagesOpened(const ImagesPtr images) {
    foreach (auto listener, listeners) {
        listener->onImagesOpened(images);
    }
}

void ImageFilesInteractors::notifyImagesClosed() {
    foreach (auto listener, listeners) {
        listener->onImagesClosed();
    }
}

void ImageFilesInteractors::notifyImagesOpenFailed(const QString &error) {
    foreach (auto listener, listeners) {
        listener->onImagesOpenFailed(error);
    }
}

void ImageFilesInteractors::notifySavingFileFailed(const QString &path) {
    foreach (auto listener, listeners) {
        listener->onSavingFileFailed(path);
    }
}

void ImageFilesInteractors::notifyFileSavedSuccessfully(const QString &path) {
    foreach (auto listener, listeners) {
        listener->onFileSavedSuccessfully(path);
    }
}
