#ifndef IMAGEVALIDATIONRULESFACTORY_H
#define IMAGEVALIDATIONRULESFACTORY_H

#include <business/validation/imageextensionvalidator.h>
#include <domain/valueobjects/images.h>
#include <business/validation/interfaces/iimagevalidationrules.h>
#include <business/validation/imagevalidationrules.h>

class ImageValidationRulesFactory {
public:
    ImageValidationRulesFactory() = delete;
    ~ImageValidationRulesFactory() = delete;

    static std::shared_ptr<IImageValidationRules> createImageFormatValidator(ImagesPtr images) {
        return std::make_shared<ImageValidationRules>(images);
    }

    static std::shared_ptr<IImageExtentionValidator> createImageExtensionValidator() {
        return std::make_shared<ImageExtensionsInfoProvider>();
    }
};

#endif // IMAGEVALIDATIONRULESFACTORY_H
