#include "comparisionresultvariant.h"

ComparisionResultVariant::ComparisionResultVariant(){
    this->_type = ComparisionResultVariantType::None;
}

ComparisionResultVariant::ComparisionResultVariant(QImage image) {
    this->image = image;
    this->_type = ComparisionResultVariantType::Image;
}

ComparisionResultVariant::ComparisionResultVariant(QString string) {
    this->string = string;
    this->_type = ComparisionResultVariantType::String;
}

QImage ComparisionResultVariant::imageResult() {
    if (_type != ComparisionResultVariantType::Image) {
        throw std::runtime_error("An incorrect ComparisionResultVariant type.");
    } else {
        return image;
    }
}

QString ComparisionResultVariant::stringResult() {
    if (_type != ComparisionResultVariantType::String) {
        throw std::runtime_error("An incorrect ComparisionResultVariant type.");
    } else {
        return string;
    }
}

ComparisionResultVariantType ComparisionResultVariant::type() {
    return _type;
}
