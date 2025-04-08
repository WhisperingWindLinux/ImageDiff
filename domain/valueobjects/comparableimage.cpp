#include "comparableimage.h"

#include <qfileinfo.h>
#include <qpixmap.h>

ComparableImage::ComparableImage(const QImage &image, const QString &imageName)
    : mImage(image),
    mImageName(imageName)
{
}

ComparableImage::ComparableImage(QImage&& image, QString&& imageName)
    : mImage(std::move(image)),
    mImageName(std::move(imageName))
{
}

ComparableImage::ComparableImage(const QPixmap &image, const QString &imageName) {
    mImage = image.toImage();
    mImageName = imageName;
}

QImage ComparableImage::getImage() const {
    return mImage;
}

QString ComparableImage::getPath() const {
    return mImageName;
}

QString ComparableImage::getImageName() const {
    return mImageName;
}
