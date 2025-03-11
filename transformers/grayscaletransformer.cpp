#include "grayscaletransformer.h"

#include <QtCore/qdebug.h>

#include <imageprocessorsmanager/property.h>


GrayscaleTransformer::GrayscaleTransformer() {

}

QString GrayscaleTransformer::name() {
    return "Make Grayscale";
}

QString GrayscaleTransformer::hotkey() {
    return "l";
}

QString GrayscaleTransformer::description() {
    return "Convert an image to grayscale.";
}

QImage GrayscaleTransformer::transform(QImage image) {
    // Create a copy of the input image to modify
    QImage grayImage = image;

    // Loop through all the pixels of the image
    for (int y = 0; y < grayImage.height(); ++y) {
        for (int x = 0; x < grayImage.width(); ++x) {
            // Get the original color of the pixel
            QColor color = grayImage.pixelColor(x, y);

            // Calculate the grayscale value using luminosity method
            int grayValue = qGray(color.red(), color.green(), color.blue());

            // Set the pixel to the new grayscale value
            grayImage.setPixelColor(x, y, QColor(grayValue, grayValue, grayValue));
        }
    }

    return grayImage;
}

