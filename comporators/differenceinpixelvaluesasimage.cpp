#include <QPainter>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

#include "differenceinpixelvaluesasimage.h"

QImage DifferenceInPixelValuesAsImageComporator::compareImages(QImage image1, QImage image2) {
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

QString DifferenceInPixelValuesAsImageComporator::getShortName() const {
    return "Difference In Pixel Values As An Image";
}

QString DifferenceInPixelValuesAsImageComporator::getFullName() const {
    return "Difference in pixel values as an image";
}

QString DifferenceInPixelValuesAsImageComporator::getHotkey() const {
    return "D";
}

QString DifferenceInPixelValuesAsImageComporator::getDescription() const {
    return QString("Show the difference in pixel values as an image. "
                   "Pixels that differ are marked with red dots.");
}

std::shared_ptr<ComparisonResultVariant> DifferenceInPixelValuesAsImageComporator::compare(ComparableImage first, ComparableImage second) {
    auto result = compareImages(first.getImage(), second.getImage());
    std::shared_ptr<ComparisonResultVariant> resultVariant = std::make_shared<ComparisonResultVariant>(result);
    return resultVariant;
}
