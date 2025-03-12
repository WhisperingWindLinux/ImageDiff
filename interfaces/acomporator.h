#ifndef ACOMPORATOR_H
#define ACOMPORATOR_H

#include <qstring.h>

#include <imageprocessorsmanager/comparableimage.h>
#include <imageprocessorsmanager/comparisonresultvariant.h>
#include <interfaces/aimageprocessor.h>

/*
 * A base class for all comparators' interfaces in the app
 */

class AComparator : public AImageProcessor {
public:
    virtual std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) = 0;

    virtual QList<Property> getDefaultProperties() const override { return {}; }

    virtual void setProperties(QList<Property>) override { };

    ImageProcessorType getType() const override { return ImageProcessorType::Comparator; }
};

#endif // ACOMPORATOR_H


