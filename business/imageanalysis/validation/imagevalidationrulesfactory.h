#ifndef IMAGEVALIDATIONRULESFACTORY_H
#define IMAGEVALIDATIONRULESFACTORY_H

#include "business/imageanalysis/validation/imagevalidationrules.h"
#include "business/imageanalysis/validation/interfaces/iimagevalidationrules.h"
#include "domain/valueobjects/images.h"

class ImageValidationRulesFactory {
public:
    ImageValidationRulesFactory() = delete;
    ~ImageValidationRulesFactory() = delete;

    static std::shared_ptr<IImageValidationRules> create(ImagesPtr images) {
        return std::make_shared<ImageValidationRules>(images);
    }
};

#endif // IMAGEVALIDATIONRULESFACTORY_H
