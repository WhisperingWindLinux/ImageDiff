#ifndef DIFFERENCEINPIXELVALUESASIMAGE_H
#define DIFFERENCEINPIXELVALUESASIMAGE_H

#include <qimage.h>
#include <qstring.h>

#include <domain/interfaces/comparator.h>

// This class shows the difference in pixel values as an image.
// Pixels that differ are marked with red dots.

class DifferenceInPixelValuesAsImageComporator : public IComparator
{
public:
    DifferenceInPixelValuesAsImageComporator() = default;
    virtual ~DifferenceInPixelValuesAsImageComporator() = default;

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

#endif // DIFFERENCEINPIXELVALUESASIMAGE_H
