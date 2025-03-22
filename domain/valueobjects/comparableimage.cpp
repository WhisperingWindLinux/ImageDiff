#include "comparableimage.h"

#include <qfileinfo.h>
#include <qpixmap.h>

ComparableImage::ComparableImage(const QImage &image, const QString &name)
    : image(image),
    name(name)
{
}

ComparableImage::ComparableImage(QImage&& image, QString&& name)
    : image(std::move(image)),
    name(std::move(name))
{
}

ComparableImage::ComparableImage(const QPixmap &image, const QString &name) {
    this->image = image.toImage();
    this->name = name;
}

const QImage& ComparableImage::getImage() const {
    return image;
}

const QString& ComparableImage::getName() const {
    return name;
}
