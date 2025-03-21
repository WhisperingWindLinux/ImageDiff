#include "imagefilesinteractors.h"

#include "business/getimagesfromvideosinteractor.h"
#include "recentfilesinteractor.h"
#include <data/storage/imagefileshandler.h>

ImageFilesInteractor::ImageFilesInteractor() {
    imageFileHandler = new ImageFilesHandler();
    recentFilesInteractor = new RecentFilesInteractor();
}

ImageFilesInteractor::~ImageFilesInteractor() {
    if (recentFilesInteractor != nullptr) {
        delete recentFilesInteractor;
        recentFilesInteractor = nullptr;
    }
    if (imageFileHandler != nullptr) {
        delete imageFileHandler;
        imageFileHandler = nullptr;
    }
}

void ImageFilesInteractor::openImagesFromRecentMenu(const QString &recentFileMenuRecord) {
    try {
        auto paths = recentFilesInteractor->getRecentFilesPathsByRecentMenuRecord(recentFileMenuRecord);
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

void ImageFilesInteractor::openImagesFromDragAndDrop(const QList<QUrl> &urls) {
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

void ImageFilesInteractor::openImages(const QString &image1Path,
                                      const QString &image2Path
                                     )
{
    try {
        ImagesPtr images = imageFileHandler->openImages(image1Path, image2Path);
        if (images == nullptr) {
            throw std::runtime_error("Unknown error.");
        }
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        notifyImagesOpenFailed(e.what());
    }
}

void ImageFilesInteractor::openImagesViaCommandLine(const QString &image1Path,
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

void ImageFilesInteractor::openImagesViaOpenFilesDialog() {
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

void ImageFilesInteractor::saveImage(const SaveImageInfo &info) {
    FileSaveResult result = imageFileHandler->saveImage(info, images);
    if (result.isSaved) {
        notifyFileSavedSuccessfully(result.path);
    } else {
        notifySavingFileFailed(result.path);
    }
}

bool ImageFilesInteractor::subscribe(IImageFilesInteractorListener *listener) {
    if (listener == nullptr) {
        return false;
    }
    if (listeners.contains(listener)) {
        return false;
    }
    listeners.append(listener);
    return true;
}

bool ImageFilesInteractor::unsubscribe(const IImageFilesInteractorListener *listener) {
    if (listener == nullptr) {
        return false;
    }
    return listeners.removeOne(listener);
}

void ImageFilesInteractor::openImagesFromVideos() {
    try {
        GetImagesFromVideosInteractor getImagesFromVideosInteractor {};
        ImagesPtr imagesPath = getImagesFromVideosInteractor.get();
        ImagesPtr images = imageFileHandler->openImages(imagesPath->path1, imagesPath->path2);
        if (images == nullptr) {
            throw std::runtime_error("Unknown error.");
        }
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        notifyImagesOpenFailed(e.what());
    }
}

void ImageFilesInteractor::notifyImagesOpened(const ImagesPtr images) {
    foreach (auto listener, listeners) {
        listener->onImagesOpened(images);
    }
}

void ImageFilesInteractor::notifyImagesClosed() {
    foreach (auto listener, listeners) {
        listener->onImagesClosed();
    }
}

void ImageFilesInteractor::notifyImagesOpenFailed(const QString &error) {
    foreach (auto listener, listeners) {
        listener->onImagesOpenFailed(error);
    }
}

void ImageFilesInteractor::notifySavingFileFailed(const QString &path) {
    foreach (auto listener, listeners) {
        listener->onSavingFileFailed(path);
    }
}

void ImageFilesInteractor::notifyFileSavedSuccessfully(const QString &path) {
    foreach (auto listener, listeners) {
        listener->onFileSavedSuccessfully(path);
    }
}
