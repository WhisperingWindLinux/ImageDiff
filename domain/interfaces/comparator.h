#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <qstring.h>

#include <domain/valueobjects/comparableimage.h>
#include <domain/valueobjects/comparisonresultvariant.h>
#include <domain/interfaces/imageprocessor.h>

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

    // The user can generate a report by running all supported comparators
    // in automatic mode through the menu Analysis -> Run Analysis.
    // In this case, all comparators that return true will be executed.
    // In this mode, the comparator cannot interact with the user through
    // the 'properties' mechanism in any way.
    virtual bool isPartOfAutoReportingToolbox() { return true; }
};

#endif // COMPARATOR_H


