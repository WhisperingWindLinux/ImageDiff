#include "comparisonresultvariant.h"

ComparisonResultVariant::ComparisonResultVariant(){
    this->_type = ComparisonResultVariantType::None;
}

ComparisonResultVariant::ComparisonResultVariant(QImage image) {
    this->image = image;
    this->_type = ComparisonResultVariantType::Image;
}

ComparisonResultVariant::ComparisonResultVariant(QString string) {
    this->string = string;
    this->_type = ComparisonResultVariantType::String;
}

QImage ComparisonResultVariant::imageResult() {
    if (_type != ComparisonResultVariantType::Image) {
        throw std::runtime_error("An incorrect ComparisionResultVariant type.");
    } else {
        return image;
    }
}

QString ComparisonResultVariant::stringResult() {
    if (_type != ComparisonResultVariantType::String) {
        throw std::runtime_error("An incorrect ComparisionResultVariant type.");
    } else {
        return string;
    }
}

ComparisonResultVariantType ComparisonResultVariant::type() {
    return _type;
}
