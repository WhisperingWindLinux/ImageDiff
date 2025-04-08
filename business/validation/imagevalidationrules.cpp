#include "imagevalidationrules.h"

#include <qimageiohandler.h>
#include <qimageiohandler.h>


ImageValidationRules::ImageValidationRules(const ImageHolderPtr images)
    : mImages(images)
{
}

ImageValidationRules::~ImageValidationRules() {
    mImages = nullptr;
}

std::optional<QString> ImageValidationRules::isValid() {
    if (mImages->isSingleImage()) {
        return isSingleImageValid();
    } else {
        return isTwoImagesValid();
    }
}

std::optional<QString> ImageValidationRules::isSingleImageValid() {
    if (mImages->getFirstImage().isNull() ) {
        return "Image is empty.";
    }
    if (!isImage8Bit(mImages->getFirstImage())) {
        return "The first image is not 8-bit.";
    }
    return std::nullopt;
}

std::optional<QString> ImageValidationRules::isTwoImagesValid() {
    if (mImages->getFirstImage().isNull() || mImages->getSecondImage().isNull()) {
        return "One or both images are empty.";
    }
    if (!isImage8Bit(mImages->getFirstImage())) {
        return "The first image is not 8-bit.";
    }
    if (!isImage8Bit(mImages->getSecondImage())) {
        return "The second image is not 8-bit.";
    }
    if (mImages->getFirstImage().depth() != mImages->getSecondImage().depth()) {
        return "The images do not have the same bit depth.";
    }
    if (mImages->getFirstImage().size() != mImages->getSecondImage().size()) {
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
