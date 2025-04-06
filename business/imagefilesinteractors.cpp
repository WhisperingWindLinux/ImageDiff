#include "imagefilesinteractors.h"

#include "business/getimagesfromvideosinteractor.h"
#include "recentfilesinteractor.h"
#include <QtCore/qmimedata.h>
#include <QtGui/qclipboard.h>
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
    images = nullptr;
}

void ImageFilesInteractor::openImagesFromRecentMenu(const QString &recentFileMenuRecord) {
    try {
        auto paths = recentFilesInteractor->getRecentFilesPathsByRecentMenuRecord(recentFileMenuRecord);
        if (!paths) {
            throw std::runtime_error("Incorrect path to one or both images.");
        }
        images = imageFileHandler->openImages(paths->first, paths->second);
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImagesFromDragAndDrop(const QList<QUrl> &urls) {
    try {
        images = imageFileHandler->openImages(urls);
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImages(const QString &image1Path,
                                      const QString &image2Path
                                     )
{
    try {
        images = imageFileHandler->openImages(image1Path, image2Path);
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImagesViaCommandLine(const QString &image1Path,
                                                    const QString &image2Path
                                                   )
{
    try {
        images = imageFileHandler->openImages(image1Path, image2Path);

        if (isFileInTempFolder(images->path1) && isFileInTempFolder(images->path2)) {
            images->markAsTemporary();
        }
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImagesViaOpenFilesDialog() {
    try {
        images = imageFileHandler->openImages();
        if (images != nullptr) {
            notifyImagesOpened(images);
        }
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImageViaOpenFilesDialog() {
    try {
        images = imageFileHandler->openImage();
        if (images != nullptr) {
            notifyImagesOpened(images);
        }
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImagesFromVideos() {
    try {
        GetImagesFromVideosInteractor getImagesFromVideosInteractor {};
        ImagesPtr imagesPath = getImagesFromVideosInteractor.get();
        if (imagesPath == nullptr) {
            return;
        }
        images = imageFileHandler->openImages(imagesPath->path1, imagesPath->path2);
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImageFromClipboard() {
    try {
        std::string errorMissingImage = "The clipboard does not contain an image, "
                                        "or the image format is not supported.";

        QClipboard *clipboard = QApplication::clipboard();
        if (!clipboard->mimeData()->hasImage()) {
            throw std::runtime_error(errorMissingImage);
        }
        QImage image = clipboard->image();
        if (image.isNull()) {
            throw std::runtime_error(errorMissingImage);
        }
        QPixmap pixmap = QPixmap::fromImage(image);
        auto result = imageFileHandler->saveImageTemporary(pixmap);
        if (!result) {
            throw std::runtime_error("Unable to save the image in the Temp directory");
        }
        auto images = imageFileHandler->openImages(result->path, result->path);
        images->markAsTemporary();
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::saveImage(const SaveImageInfo &info) {
    auto result = imageFileHandler->saveImageAs(info, images);
    if (result.has_value()) {
        return;
    }
    if (result.value().isSaved) {
        notifyFileSavedSuccessfully(result.value().path);
    } else {
        notifySavingFileFailed(result.value().path);
    }
}

void ImageFilesInteractor::saveImageAs(const SaveImageInfo &info) {
    auto result = imageFileHandler->saveImageAs(info, images);
    if (result.has_value()) {
        return;
    }
    if (result.value().isSaved) {
        notifyFileSavedSuccessfully(result.value().path);
    } else {
        notifySavingFileFailed(result.value().path);
    }
}

bool ImageFilesInteractor::isFileInTempFolder(const QString &filePath) {
    QString tempPath = QDir::tempPath();
    QFileInfo fileInfo(filePath);
    return fileInfo.absoluteFilePath().startsWith(tempPath) && fileInfo.isFile();
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

void ImageFilesInteractor::cleanup() {
    images = nullptr;
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
