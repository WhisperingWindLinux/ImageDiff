#ifndef MONOCOLOREDDIFFERENCEINPIXELVALUESCOMPORATOR_H
#define MONOCOLOREDDIFFERENCEINPIXELVALUESCOMPORATOR_H

#include <qimage.h>
#include <qstring.h>

#include <domain/interfaces/business/icomparator.h>

// This class shows the difference in pixel values as an image.
// Pixels that differ are marked with red dots.

class MonoColoredDifferenceInPixelValuesComporator : public IComparator
{
public:
    MonoColoredDifferenceInPixelValuesComporator() = default;
    virtual ~MonoColoredDifferenceInPixelValuesComporator() = default;

    // IComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    ComparisonResultVariantPtr compare(const ComparableImage &first,
                                       const ComparableImage &second) override;
    QString getFullName() const override;

private:
    QImage compareImages(const QImage &image1, const QImage &image2);
};

#endif // MONOCOLOREDDIFFERENCEINPIXELVALUESCOMPORATOR_H
