#include "grayscalefilter.h"

#include <QtCore/qdebug.h>

QString GrayscaleFilter::getShortName() const {
    return "Make Grayscale";
}

QString GrayscaleFilter::getFullName() const {
    return getShortName();
}

QString GrayscaleFilter::getHotkey() const {
    return "l";
}

QString GrayscaleFilter::getDescription() const {
    return "Convert an image to grayscale.";
}

QImage GrayscaleFilter::filter(const QImage &image) {
    QImage grayImage = image;

    for (int y = 0; y < grayImage.height(); ++y) {
        for (int x = 0; x < grayImage.width(); ++x) {
            QColor color = grayImage.pixelColor(x, y);

            // Calculate the grayscale value using luminosity method
            int grayValue = qGray(color.red(), color.green(), color.blue());

            grayImage.setPixelColor(x, y, QColor(grayValue, grayValue, grayValue));
        }
    }

    return grayImage;
}
