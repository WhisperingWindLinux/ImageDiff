#include "comparableimage.h"

#include <qfileinfo.h>
#include <qpixmap.h>

ComparableImage::ComparableImage(QImage image, QString name) {
    this->image = image;
    this->name = QFileInfo(name).fileName();
}

ComparableImage::ComparableImage(QPixmap image, QString name) {
    this->image = image.toImage();
    this->name = QFileInfo(name).fileName();
}

QImage ComparableImage::getImage() const {
    return image;
}

QString ComparableImage::getName() const {
    return name;
}
