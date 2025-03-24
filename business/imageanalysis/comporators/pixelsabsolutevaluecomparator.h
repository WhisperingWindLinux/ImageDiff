#ifndef PIXELSABSOLUTEVALUECOMPARATOR_H
#define PIXELSABSOLUTEVALUECOMPARATOR_H

#include <QPair>

#include <domain/interfaces/comparator.h>

#include <domain/valueobjects/pixeldiffrencerange.h>


// This class calculates the difference in RGB values between two images.
// It then displays the differences as a list of ranges along with the corresponding pixel counts.

class PixelsAbsoluteValueComparator : public IComparator
{
public:
    enum class Mode { DifferenceBySingleLargestComponent, DifferenceByAllComponents };

public:
    PixelsAbsoluteValueComparator();
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

    QList<PixelDifferenceRange> compareImages(const QImage &image1, const QImage &image2);
    int calculateDiff(QColor color1, QColor color2);
};

#endif // PIXELSABSOLUTEVALUECOMPARATOR_H
