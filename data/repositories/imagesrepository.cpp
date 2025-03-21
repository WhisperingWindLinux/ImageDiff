#include "imagesrepository.h"

#include <QtCore/qdir.h>
#include <data/storage/imagefileshandler.h>
#include <qfileinfo.h>

ImagesRepository::ImagesRepository(const QString &image1Path,
                                   const QString &image2Path,
                                   const QPixmap &image1,
                                   const QPixmap &image2
                                   )
    : isTemporaryFiles(false)
{
    QFileInfo firstImageInfo { image1Path };
    QFileInfo secondImageInfo { image2Path };

    firstImagePath = image1Path;
    secondImagePath = image2Path;

    originalFirstImage = image1;
    originalSecondImage = image2;
    firstCurrentlyDisplayedImage = originalFirstImage;
    secondCurrentlyDisplayedImage = originalSecondImage;

    firstImageName = firstImageInfo.fileName();
    secondImageName = secondImageInfo.fileName();
    firstImageBaseName = firstImageInfo.baseName();
    secondImageBaseName = secondImageInfo.baseName();
    firstImageDir = firstImageInfo.dir().absolutePath();
    secondImageDir = secondImageInfo.dir().absolutePath();
}

ImagesRepository::~ImagesRepository() {
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

// Replace the old two compared images with new ones that have image processing
// algorithms (filters) applied. At the same time, we always keep a copy of
// the original files so that all changes can be rolled back at the user's
// request.
void ImagesRepository::update(const QPixmap &image1, const QPixmap &image2) {
    firstCurrentlyDisplayedImage = image1;
    secondCurrentlyDisplayedImage = image2;
}

// The user applied an image processing algorithm (comparator) that
// does not alter the two original compared images but creates a third
// image, which is the result of comparing the two originals.
// It is displayed as an additional (third) image in the viewer.
void ImagesRepository::update(const QPixmap &comparisonResultImage) {
    displayedComparisonResultImage = comparisonResultImage;
}

// When the user selects an area of the image with the mouse while holding
// down Command, a new instance of the application opens with cropped images.
// These images are saved in the Temp folder and will be deleted upon closing
// the application unless the user saves them.
void ImagesRepository::setDeleteFilesAfterClose() {
    isTemporaryFiles = true;
}

// The user requested the removal of all applied filters
// from the two compared images.
void ImagesRepository::restoreOriginalImages() {
    firstCurrentlyDisplayedImage = originalFirstImage;
    secondCurrentlyDisplayedImage = originalFirstImage;
    displayedComparisonResultImage = {};
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
