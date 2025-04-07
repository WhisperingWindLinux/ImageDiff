#ifndef IMAGEVALIDATIONRULESFACTORY_H
#define IMAGEVALIDATIONRULESFACTORY_H

#include <business/validation/imageextensionsinfoprovider.h>
#include <domain/valueobjects/images.h>
#include <business/validation/interfaces/iimagevalidationrules.h>
#include <business/validation/imagevalidationrules.h>

class ImageValidationRulesFactory {
public:
    ImageValidationRulesFactory() = delete;
    ~ImageValidationRulesFactory() = delete;

    static std::shared_ptr<IImageValidationRules> createImageFormatValidator(ImageHolderPtr images) {
        return std::make_shared<ImageValidationRules>(images);
    }

    static std::shared_ptr<IImageExtensionsInfoProvider> createImageExtensionsInfoProvider() {
        return std::make_shared<ImageExtensionsInfoProvider>();
    }
};

#endif // IMAGEVALIDATIONRULESFACTORY_H
