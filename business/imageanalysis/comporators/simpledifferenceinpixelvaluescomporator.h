#ifndef SIMPLEDIFFERENCEINPIXELVALUESCOMPORATOR_H
#define SIMPLEDIFFERENCEINPIXELVALUESCOMPORATOR_H

#include <qimage.h>
#include <qstring.h>

#include <domain/interfaces/comparator.h>

// This class shows the difference in pixel values as an image.
// Pixels that differ are marked with red dots.

class SimpleDifferenceInPixelValuesComporator : public IComparator
{
public:
    SimpleDifferenceInPixelValuesComporator() = default;
    virtual ~SimpleDifferenceInPixelValuesComporator() = default;

    // AComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    ComparisonResultVariantPtr compare(const ComparableImage &first,
                                       const ComparableImage &second) override;
    QString getFullName() const override;

private:
    QImage compareImages(const QImage &image1, const QImage &image2);
};

#endif // SIMPLEDIFFERENCEINPIXELVALUESCOMPORATOR_H
