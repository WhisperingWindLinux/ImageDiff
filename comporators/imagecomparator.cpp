#include <QPainter>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

#include "imagecomparator.h"


// Method to compare images and save the result
QImage ImageComparator::compareImages(QImage image1, QImage image2) {
    if (image1.isNull() || image2.isNull()) {
        throw std::runtime_error("Failed to load one of the images.");
    }

    // Check if image sizes match
    if (image1.size() != image2.size()) {
        throw std::runtime_error("Image sizes do not match!");
    }

    // Create a resulting image (copy of the first image)
    QImage resultImg = image1.copy();
    QPainter painter(&resultImg);

    // Add a semi-transparent layer
    painter.setOpacity(0.6);
    painter.drawImage(0, 0, image1);
    painter.setOpacity(1.0);

    // Compare pixels and highlight differences in red
    for (int y = 0; y < image1.height(); ++y) {
        for (int x = 0; x < image1.width(); ++x) {
            if (image1.pixel(x, y) != image2.pixel(x, y)) {
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

std::shared_ptr<ComparisonResultVariant> ImageComparator::compare(ComparableImage first, ComparableImage second) {
    auto result = compareImages(first.getImage(), second.getImage());
    std::shared_ptr<ComparisonResultVariant> resultVariant = std::make_shared<ComparisonResultVariant>(result);
    return resultVariant;
}
