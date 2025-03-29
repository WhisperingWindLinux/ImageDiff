#ifndef ICOMPARATOR_H
#define ICOMPARATOR_H

#include <qstring.h>

#include <domain/valueobjects/comparableimage.h>
#include <domain/valueobjects/comparisonresultvariant.h>
#include <domain/interfaces/business/imageprocessor.h>

/*
 * A base class for all comparators' interfaces in the app
 */

class IComparator : public IImageProcessor {
public:
    virtual ComparisonResultVariantPtr compare(const ComparableImage &first,
                                               const ComparableImage &second) = 0;

    virtual QList<Property> getDefaultProperties() const override;

    virtual void setProperties(QList<Property>) override;;

    ImageProcessorType getType() const override;

    // The user can generate a report by running all supported comparators
    // in automatic mode through the menu Analysis -> Run Analysis.
    // In this case, all comparators that return true will be executed.
    // In this mode, the comparator cannot interact with the user through
    // the 'properties' mechanism in any way.
    virtual bool isPartOfAutoReportingToolbox();

    bool isEnabled();

    void setEnabled(bool isEnabled);

private:
    bool m_isEnabled = true;
};

typedef std::shared_ptr<IComparator> IComparatorPtr;

#endif // ICOMPARATOR_H


