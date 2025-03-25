#ifndef CUSTOMRANGEDDIFFERENCEINPIXELVALUESCOMPARATOR_H
#define CUSTOMRANGEDDIFFERENCEINPIXELVALUESCOMPARATOR_H

#include "business/imageanalysis/comporators/helpers/pixelsasolutvaluehelper.h"
#include <domain/interfaces/business/icomparator.h>


class CustomRangedDifferenceInPixelValuesComparator : public IComparator
{
public:
    CustomRangedDifferenceInPixelValuesComparator();

    // IComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    QString getFullName() const override;
    ComparisonResultVariantPtr compare(const ComparableImage &first, const ComparableImage &second) override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property> properties) override;
    void reset() override;
    bool isPartOfAutoReportingToolbox() override;

private:
    PixelsDifferenceCalculationMode currentMode;
    int startOfRange, endOfRange;
};

#endif // CUSTOMRANGEDDIFFERENCEINPIXELVALUESCOMPARATOR_H
