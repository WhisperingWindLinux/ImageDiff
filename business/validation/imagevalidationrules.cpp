#include "imagevalidationrules.h"

#include <qimageiohandler.h>
#include <qimageiohandler.h>


ImageValidationRules::ImageValidationRules(const ImagesPtr images)
    : images(images)
{
}

ImageValidationRules::~ImageValidationRules() {
    images = nullptr;
}

std::optional<QString> ImageValidationRules::isValid() {
    // Check if both images are not null
    if (images->image1.isNull() || images->image2.isNull()) {
        return "One or both images are empty.";
    }

    // Check if the first image is 8-bit
    if (!isImage8Bit(images->image1)) {
        return "The first image is not 8-bit.";
    }

    // Check if the second image is 8-bit
    if (!isImage8Bit(images->image2)) {
        return "The second image is not 8-bit.";
    }

    // Check if both images have the same bit depth
    if (images->image1.depth() != images->image2.depth()) {
        return "The images do not have the same bit depth.";
    }

    // Check if both images have the same size
    if (images->image1.size() != images->image2.size()) {
        return "The images do not have the same dimensions.";
    }

    return std::nullopt;
}


bool ImageValidationRules::isImage8Bit(const QPixmap &pixmap) {
    QImage image = pixmap.toImage();
    if (image.hasAlphaChannel()) {
        return image.depth() == 32;
    } else {
        return image.depth() == 24 || image.depth() == 32;
    }
}
