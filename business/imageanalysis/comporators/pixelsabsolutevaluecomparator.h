#ifndef PIXELSABSOLUTEVALUECOMPARATOR_H
#define PIXELSABSOLUTEVALUECOMPARATOR_H

#include "domain/valueobjects/comparisonresultvariant.h"
#include <domain/valueobjects/pixeldiffrencerange.h>
#include <domain/valueobjects/property.h>
#include <domain/interfaces/comparator.h>

class PixelsAbsoluteValueComparator : public IComparator
{
public:
    enum class Mode { DifferenceBySingleLargestComponent, DifferenceByAllComponents };
    enum class Result { Text, Image };

public:
    PixelsAbsoluteValueComparator(PixelsAbsoluteValueComparator::Result result);
    virtual ~PixelsAbsoluteValueComparator() = default;

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

#endif // PIXELSABSOLUTEVALUECOMPARATOR_H


