#include "comparableimage.h"

#include <qfileinfo.h>
#include <qpixmap.h>

ComparableImage::ComparableImage(const QImage &image, const QString &path)
    : image(image),
    path(path)
{
}

ComparableImage::ComparableImage(QImage&& image, QString&& path)
    : image(std::move(image)),
    path(std::move(path))
{
}

ComparableImage::ComparableImage(const QPixmap &image, const QString &path) {
    this->image = image.toImage();
    this->path = path;
}

QImage ComparableImage::getImage() const {
    return image;
}

QString ComparableImage::getPath() const {
    return path;
}

QString ComparableImage::getBaseName() const {
    QFileInfo file {path};
    return file.baseName();
}
