#include <QPainter>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

#include "imagecomparator.h"


// Method to compare images and save the result
QImage ImageComparator::compareImages(const QString imagePath1, const QString imagePath2) {
    // Load images
    QImage img1(imagePath1);
    QImage img2(imagePath2);

    if (img1.isNull() || img2.isNull()) {
        throw std::runtime_error("Failed to load one of the images.");
    }

    // Check if image sizes match
    if (img1.size() != img2.size()) {
        throw std::runtime_error("Image sizes do not match!");
    }

    // Create a resulting image (copy of the first image)
    QImage resultImg = img1.copy();
    QPainter painter(&resultImg);

    // Add a semi-transparent layer
    painter.setOpacity(0.5);
    painter.drawImage(0, 0, img1);
    painter.setOpacity(1.0);

    // Compare pixels and highlight differences in red
    for (int y = 0; y < img1.height(); ++y) {
        for (int x = 0; x < img1.width(); ++x) {
            if (img1.pixel(x, y) != img2.pixel(x, y)) {
                resultImg.setPixelColor(x, y, QColor(255, 0, 0, 255)); // Red color
            }
        }
    }

    painter.end();
    return resultImg;
}

QString ImageComparator::name() {
    return "Show the difference in pixel values as an image";
}

QString ImageComparator::hotkey() {
    return "D";
}

QString ImageComparator::description() {
    return QString("Show the difference in pixel values as an image. "
                   "Pixels that differ are marked with red dots.");
}

std::shared_ptr<ComparisionResultVariant> ImageComparator::compare(QList<QString> filesPath) {
    if (filesPath.size() != 2) {
        return std::make_shared<ComparisionResultVariant>();
    }
    QImage result = compareImages(filesPath[0], filesPath[1]);
    std::shared_ptr<ComparisionResultVariant> resultVariant = std::make_shared<ComparisionResultVariant>(result);
    return resultVariant;
}

ComporatorContentType ImageComparator::contentType() {
    return ComporatorContentType::Image;
}
