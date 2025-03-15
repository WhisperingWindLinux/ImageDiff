#ifndef DIFFERENCEINPIXELVALUESASIMAGE_H
#define DIFFERENCEINPIXELVALUESASIMAGE_H

#include <qimage.h>
#include <qstring.h>

#include <interfaces/comporator.h>

// This class shows the difference in pixel values as an image.
// Pixels that differ are marked with red dots.

class DifferenceInPixelValuesAsImageComporator : public IComparator
{
public:

    DifferenceInPixelValuesAsImageComporator() = default;
    virtual ~DifferenceInPixelValuesAsImageComporator() = default;

    // AComparator interface

    QString name() const override;
    QString hotkey() const override;
    QString htmlFormattedHelp() const override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;

private:
    QImage compareImages(QImage image1, QImage image2);
};

#endif // DIFFERENCEINPIXELVALUESASIMAGE_H
