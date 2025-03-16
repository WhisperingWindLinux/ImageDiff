#ifndef COMPORATOR_H
#define COMPORATOR_H

#include <qstring.h>

#include <imageprocessorsmanager/comparableimage.h>
#include <imageprocessorsmanager/comparisonresultvariant.h>
#include <interfaces/imageprocessor.h>

/*
 * A base class for all comparators' interfaces in the app
 */

class IComparator : public IImageProcessor {
public:
    virtual std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) = 0;

    virtual QList<Property> getDefaultProperties() const override { return {}; }

    virtual void setProperties(QList<Property>) override { };

    ImageProcessorType getType() const override { return ImageProcessorType::Comparator; }

    // The full name to be printed in an HTML report.
    virtual QString getFullName() const = 0;
};

#endif // COMPORATOR_H


