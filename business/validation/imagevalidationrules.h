#ifndef IMAGEVALIDATIONRULES_H
#define IMAGEVALIDATIONRULES_H

#include "domain/valueobjects/images.h"
#include <qfileinfo.h>
#include <business/validation/interfaces/iimagevalidationrules.h>

// Since we will be comparing the images pixel by pixel, we need to ensure
// that they have the same characteristics

class ImageValidationRules : public IImageValidationRules
{
public:
    ImageValidationRules(const ImageHolderPtr images);
    virtual ~ImageValidationRules();

    // If all checks pass, return std::nullopt
    std::optional<QString> isValid() override;

private:
    ImageHolderPtr mImages;

    std::optional<QString> isSingleImageValid();
    std::optional<QString> isTwoImagesValid();

    bool isImageColorSpaceRgb(const QPixmap &pixmap);
    bool isImage8Bit(const QPixmap &pixmap);
};

#endif // IMAGEVALIDATIONRULES_H
