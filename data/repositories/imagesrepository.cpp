#include "imagesrepository.h"

#include <QtCore/qdir.h>
#include <qfileinfo.h>

ImagesRepository::ImagesRepository()
    : imageExtentionWithoutDot("png"),
    imageExtentionWithDot(QString(".") + imageExtentionWithoutDot)
{
}

bool ImagesRepository::update(const QString &image1Path,
                              const QString &image2Path,
                              bool isTemporaryFiles
                              )
{
    clear();

    QFileInfo firstImageInfo { image1Path };
    QFileInfo secondImageInfo { image2Path };

    if (!firstImageInfo.isFile() || !secondImageInfo.isFile()) {
        return false;
    }

    QString suffix1 = "." + firstImageInfo.suffix().toLower();
    QString suffix2 = "." + secondImageInfo.suffix().toLower();
    if (suffix1 != suffix2 || suffix1 != imageExtentionWithDot) {
        return false;
    }

    originalFirstImage.load(image1Path);
    originalSecondImage.load(image2Path);

    if (originalFirstImage.isNull() || originalSecondImage.isNull()) {
        clear();
        return false;
    }

    firstImagePath = image1Path;
    secondImagePath = image2Path;
    firstCurrentlyDisplayedImage = originalFirstImage;
    secondCurrentlyDisplayedImage = originalSecondImage;

    firstImageName = firstImageInfo.fileName();
    secondImageName = secondImageInfo.fileName();
    firstImageBaseName = firstImageInfo.baseName();
    secondImageBaseName = secondImageInfo.baseName();
    firstImageDir = firstImageInfo.dir().absolutePath();
    secondImageDir = secondImageInfo.dir().absolutePath();
    this->isTemporaryFiles = isTemporaryFiles;
    return true;
}

void ImagesRepository::update(const QPixmap &image1, const QPixmap &image2) {
    firstCurrentlyDisplayedImage = image1;
    secondCurrentlyDisplayedImage = image2;
}

void ImagesRepository::update(const QPixmap &comparisonResultImage) {
    displayedComparisonResultImage = comparisonResultImage;
}

void ImagesRepository::clear() {
    if (isTemporaryFiles) {
        QFile(firstImagePath).remove();
        QFile(secondImagePath).remove();
    }
    isTemporaryFiles = false;
    originalFirstImage = {};
    originalSecondImage = {};
    firstCurrentlyDisplayedImage = {};
    secondCurrentlyDisplayedImage = {};

    firstImagePath = "";
    secondImagePath = "";
    firstImageName = "";
    secondImageName = "";
    firstImageBaseName = "";
    secondImageBaseName = "";
    firstImageDir = "";
    secondImageDir = "";
}

void ImagesRepository::restoreOriginalImages() {
    firstCurrentlyDisplayedImage = originalFirstImage;
    secondCurrentlyDisplayedImage = originalFirstImage;
    displayedComparisonResultImage = {};
}

const QString& ImagesRepository::getDefaultImageExtention(bool includeDot) const {
    if (includeDot) {
        return imageExtentionWithDot;
    } else {
        return imageExtentionWithoutDot;
    }
}

const QString& ImagesRepository::getFirstImagePath() const {
    return firstImagePath;
}

const QString& ImagesRepository::getSecondImagePath() const {
    return secondImagePath;
}

const QPixmap& ImagesRepository::getFirstImage() const {
    return firstCurrentlyDisplayedImage;
}

const QPixmap& ImagesRepository::getSecondImage() const {
    return secondCurrentlyDisplayedImage;
}

const QPixmap& ImagesRepository::getComparisonResultImage() const {
    return displayedComparisonResultImage;
}

const QString& ImagesRepository::getFirstImageBaseName() const {
    return firstImageBaseName;
}

const QString& ImagesRepository::getSecondImageBaseName() const {
    return secondImageBaseName;
}

const QString& ImagesRepository::getFirstImageDir() const {
    return firstImageDir;
}

const QString& ImagesRepository::getSecondImageDir() const {
    return secondImageDir;
}
