#ifndef ADVANCEDDIFFERENCEINPIXELVALUESCOMPORATOR_H
#define ADVANCEDDIFFERENCEINPIXELVALUESCOMPORATOR_H

#include "domain/valueobjects/comparisonresultvariant.h"
#include <domain/valueobjects/pixeldiffrencerange.h>
#include <domain/valueobjects/property.h>
#include <domain/interfaces/business/icomparator.h>

class AdvancedDifferenceInPixelValuesComporator : public IComparator
{
public:
    enum class Mode { DifferenceBySingleLargestComponent, DifferenceByAllComponents };
    enum class Result { Text, Image };

public:
    AdvancedDifferenceInPixelValuesComporator(AdvancedDifferenceInPixelValuesComporator::Result result);
    virtual ~AdvancedDifferenceInPixelValuesComporator() = default;

    // AComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    QString getFullName() const override;
    ComparisonResultVariantPtr compare(const ComparableImage &first, const ComparableImage &second) override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property> properties) override;
    void reset() override;

private:
    Mode currentMode;
    Result expectedResult;

    int calculateDiff(QColor color1, QColor color2);
};

#endif // ADVANCEDDIFFERENCEINPIXELVALUESCOMPORATOR_H


