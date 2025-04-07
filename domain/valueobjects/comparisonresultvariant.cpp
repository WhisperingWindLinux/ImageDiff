#include "comparisonresultvariant.h"

ComparisonResultVariant::ComparisonResultVariant(){
    this->mType = ComparisonResultVariantType::None;
}

ComparisonResultVariant::ComparisonResultVariant(const QImage &image) {
    this->mImage = image;
    this->mType = ComparisonResultVariantType::Image;
}

ComparisonResultVariant::ComparisonResultVariant(const QString &string) {
    this->mString = string;
    this->mType = ComparisonResultVariantType::String;
}

QImage ComparisonResultVariant::getImageResult() {
    if (mType != ComparisonResultVariantType::Image) {
        throw std::runtime_error("An internal error occurred: an incorrect ComparisionResultVariant type.");
    } else {
        return mImage;
    }
}

QString ComparisonResultVariant::getStringResult() {
    if (mType != ComparisonResultVariantType::String) {
        throw std::runtime_error("An internal error occurred: an incorrect ComparisionResultVariant type.");
    } else {
        return mString;
    }
}

ComparisonResultVariantType ComparisonResultVariant::getType() {
    return mType;
}
