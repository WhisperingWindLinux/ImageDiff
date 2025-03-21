#include "imagefilesinteractors.h"

#include "recentfilesinteractor.h"
#include <data/storage/imagefileshandler.h>
#include <data/repositories/imagesrepository.h>

ImageFilesInteractors::ImageFilesInteractors(IMainWindowCallbacks *callbacks)
    : callbacks(callbacks)
{
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
        ImagesPtr images = imageFileHandler->openImages(paths->first, paths->second);
        imagesRepository = std::make_shared<ImagesRepository>(paths->first,
                                                              paths->second,
                                                              images->image1,
                                                              images->image2
                                                              );
        notifyComparedImagesOpened(imagesRepository);
    } catch(std::runtime_error &e) {
        notifyComparedImagesOpenFailed(e.what());
    }
}

void ImageFilesInteractors::openImagesFromDragAndDrop(const QList<QUrl> &urls) {
    try {
        ImagesPtr images = imageFileHandler->openImages(urls);
        imagesRepository = std::make_shared<ImagesRepository>(images->path1,
                                                              images->path2,
                                                              images->image1,
                                                              images->image2
                                                              );
        notifyComparedImagesOpened(imagesRepository);
    } catch(std::runtime_error &e) {
        notifyComparedImagedsOpenFailed(e.what());
    }
}

void ImageFilesInteractors::openImagesViaCommandLine(const QString &image1Path,
                                                     const QString &image2Path
                                                     )
{
    try {
        ImagesPtr images = imageFileHandler->openImages(image1Path, image2Path);
        imagesRepository = std::make_shared<ImagesRepository>(images->path1,
                                                              images->path2,
                                                              images->image1,
                                                              images->image2
                                                              );
        imagesRepository->setDeleteFilesAfterClose();
        notifyComparedImagesOpened(imagesRepository);
    } catch(std::runtime_error &e) {
        notifyComparedImagedsOpenFailed(e.what());
    }
}

void ImageFilesInteractors::openImagesViaOpenFilesDialog() {
    try {
        ImagesPtr images = imageFileHandler->openImages();
        imagesRepository = std::make_shared<ImagesRepository>(images->path1,
                                                              images->path2,
                                                              images->image1,
                                                              images->image2
                                                              );
        notifyComparedImagesOpened(imagesRepository);
    } catch(std::runtime_error &e) {
        notifyComparedImagedsOpenFailed(e.what());
    }
}

void ImageFilesInteractors::saveImage(const SaveImageInfo &info) {
    FileSaveResult result = imageFileHandler->saveImage(info, imagesRepository);
    if (result.isSaved) {
        notifyFileSavedSuccessfully(result.path);
    } else {
        notifySaveFileFailed(result.path);
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

void ImageFilesInteractors::notifyComparedImagesOpened(IImagesRepositoryPtr imageRepository) {
    foreach (auto listener, listeners) {
        listener->onComparedImagesOpened(imageRepository);
    }
}

void ImageFilesInteractors::notifyImagesClosed() {
    foreach (auto listener, listeners) {
        listener->onImagesClosed();
    }
}

void ImageFilesInteractors::notifyComparisonResultImageOpened(IImagesRepositoryPtr imageRepository) {
    foreach (auto listener, listeners) {
        listener->onComparisonResultImageOpened(imageRepository);
    }
}

void ImageFilesInteractors::notifyComparedImagesUpdated(IImagesRepositoryPtr imageRepository) {
    foreach (auto listener, listeners) {
        listener->onComparedImagesUpdated(imageRepository);
    }
}

void ImageFilesInteractors::notifyComparedImagesOpenFailed(const QString &error) {
    foreach (auto listener, listeners) {
        listener->onComparedImagesOpenFailed(error);
    }
}

void ImageFilesInteractors::notifyComparisonResultImageOpenedFailed(const QString &error) {
    foreach (auto listener, listeners) {
        listener->onComparisonResultImageOpenedFailed(error);
    }
}

void ImageFilesInteractors::notifyComparedImagedsOpenFailed(const QString &error) {
    foreach (auto listener, listeners) {
        listener->onComparedImagedsOpenFailed(error);
    }
}

void ImageFilesInteractors::notifySaveFileFailed(const QString &path) {
    foreach (auto listener, listeners) {
        listener->onSaveFileFailed(path);
    }
}

void ImageFilesInteractors::notifyFileSavedSuccessfully(const QString &path) {
    foreach (auto listener, listeners) {
        listener->onFileSavedSuccessfully(path);
    }
}
