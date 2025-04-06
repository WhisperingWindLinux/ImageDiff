#include "icomparator.h"

QList<Property> IComparator::getDefaultProperties() const {
    return {};
}

void IComparator::setProperties(QList<Property>) {
}

ImageProcessorType IComparator::getType() const {
    return ImageProcessorType::Comparator;
}

bool IComparator::isPartOfAutoReportingToolbox() {
    return true;
}

bool IComparator::isEnabled() {
    return m_isEnabled;
}

void IComparator::setEnabled(bool isEnabled) {
    m_isEnabled = isEnabled;
}
