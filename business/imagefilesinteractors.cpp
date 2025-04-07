#include "imagefilesinteractors.h"

#include "business/getimagesfromvideosinteractor.h"
#include "recentfilesinteractor.h"
#include <QtCore/qmimedata.h>
#include <QtGui/qclipboard.h>
#include <data/storage/imagefileshandler.h>

ImageFilesInteractor::ImageFilesInteractor() {
    mImageFileHandler = new ImageFilesHandler();
    mRecentFilesInteractor = new RecentFilesInteractor();
}

ImageFilesInteractor::~ImageFilesInteractor() {
    if (mRecentFilesInteractor != nullptr) {
        delete mRecentFilesInteractor;
        mRecentFilesInteractor = nullptr;
    }
    if (mImageFileHandler != nullptr) {
        delete mImageFileHandler;
        mImageFileHandler = nullptr;
    }
    mImages = nullptr;
}

void ImageFilesInteractor::openImagesFromRecentMenu(const QString &recentFileMenuRecord) {
    try {
        auto record = mRecentFilesInteractor->getRecentFilesPathsByRecentMenuRecord(recentFileMenuRecord);
        if (!record) {
            throw std::runtime_error("Incorrect path to image(s).");
        }
        std::optional<ImageHolderPtr> images;
        if (record->isPairPathsRecord()) {
            images = mImageFileHandler->openImages(record->getFirstPath(), record->getSecondPath());
        } else {
            images = mImageFileHandler->openImage(record->getFirstPath());
        }
        if (!images) {
            return; // the user canceled the operation
        }
        mImages = images.value();
        notifyImagesOpened(mImages);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImagesFromDragAndDrop(const QList<QUrl> &urls) {
    try {
        mImages = mImageFileHandler->openImages(urls);
        notifyImagesOpened(mImages);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImages(const QString &firstImagePath,
                                      const QString &secondImagePath
                                     )
{
    try {
        mImages = mImageFileHandler->openImages(firstImagePath, secondImagePath);
        notifyImagesOpened(mImages);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImagesViaCommandLine(const QString &firstImagePath,
                                                    const QString &secondImagePath
                                                   )
{
    try {
        mImages = mImageFileHandler->openImages(firstImagePath, secondImagePath);

        if (isFileInTempFolder(mImages->getFirstImagePath()) &&
            isFileInTempFolder(mImages->getSecondImagePath()))
        {
            mImages->markTemporary();
        }
        notifyImagesOpened(mImages);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImagesViaOpenFilesDialog() {
    try {
        mImages = mImageFileHandler->openImages();
        if (mImages != nullptr) {
            notifyImagesOpened(mImages);
        }
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::openImageViaOpenFilesDialog() {
    try {
        mImages = mImageFileHandler->openImage();
        if (mImages != nullptr) {
            notifyImagesOpened(mImages);
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
        ImageHolderPtr imagesPath = getImagesFromVideosInteractor.get();
        if (imagesPath == nullptr) {
            return;
        }
        mImages = mImageFileHandler->openImages(imagesPath->getFirstImagePath(),
                                                imagesPath->getSecondImagePath()
                                                );
        notifyImagesOpened(mImages);
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
        auto path = mImageFileHandler->saveImageAsTemporary(pixmap);
        auto images = mImageFileHandler->openImages(path, path);
        images->markTemporary();
        notifyImagesOpened(images);
    } catch(std::runtime_error &e) {
        cleanup();
        notifyImagesOpenFailed(e.what());
        notifyImagesClosed();
    }
}

void ImageFilesInteractor::saveImageAs(const SaveImageInfo &info) {
    std::optional<QString> path;
    try {
        auto pathPtr = mImageFileHandler->saveImageAs(info, mImages);
        if (!path) {
            return;
        }
        notifyFileSavedSuccessfully(path.value());
    } catch (std::runtime_error &e) {
        qDebug() << e.what();
        notifySavingFileFailed(path.value_or(""));
    }
}

bool ImageFilesInteractor::isFileInTempFolder(const QString &filePath) {
    QString tempPath = QDir::tempPath();
    QFileInfo fileInfo(filePath);
    return fileInfo.absoluteFilePath().startsWith(tempPath) && fileInfo.isFile();
}

void ImageFilesInteractor::notifyImagesOpened(const ImageHolderPtr images) {
    foreach (auto listener, mListeners) {
        listener->onImagesOpened(images);
    }
}

bool ImageFilesInteractor::subscribe(IImageFilesInteractorListener *listener) {
    if (listener == nullptr) {
        return false;
    }
    if (mListeners.contains(listener)) {
        return false;
    }
    mListeners.append(listener);
    return true;
}

bool ImageFilesInteractor::unsubscribe(IImageFilesInteractorListener *listener) {
    if (listener == nullptr) {
        return false;
    }
    return mListeners.removeOne(listener);
}

void ImageFilesInteractor::cleanup() {
    mImages = nullptr;
}



void ImageFilesInteractor::notifyImagesClosed() {
    foreach (auto listener, mListeners) {
        listener->onImagesClosed();
    }
}

void ImageFilesInteractor::notifyImagesOpenFailed(const QString &error) {
    foreach (auto listener, mListeners) {
        listener->onImagesOpenFailed(error);
    }
}

void ImageFilesInteractor::notifySavingFileFailed(const QString &path) {
    foreach (auto listener, mListeners) {
        listener->onSavingFileFailed(path);
    }
}

void ImageFilesInteractor::notifyFileSavedSuccessfully(const QString &path) {
    foreach (auto listener, mListeners) {
        listener->onFileSavedSuccessfully(path);
    }
}
