#include "ifilter.h"

QList<Property> IFilter::getDefaultProperties() const {
    return {};
}

void IFilter::setProperties(QList<Property>) {
}

ImageProcessorType IFilter::getType() const {
    return ImageProcessorType::Filter;
}
