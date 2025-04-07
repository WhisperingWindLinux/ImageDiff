#include "comparableimage.h"

#include <qfileinfo.h>
#include <qpixmap.h>

ComparableImage::ComparableImage(const QImage &image, const QString &path)
    : mImage(image),
    mPath(path)
{
}

ComparableImage::ComparableImage(QImage&& image, QString&& path)
    : mImage(std::move(image)),
    mPath(std::move(path))
{
}

ComparableImage::ComparableImage(const QPixmap &image, const QString &path) {
    this->mImage = image.toImage();
    this->mPath = path;
}

QImage ComparableImage::getImage() const {
    return mImage;
}

QString ComparableImage::getPath() const {
    return mPath;
}

QString ComparableImage::getBaseName() const {
    QFileInfo file {mPath};
    return file.baseName();
}
