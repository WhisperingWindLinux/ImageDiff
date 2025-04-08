#ifndef PIXELSBRIGHTNESSCOMPARATOR_H
#define PIXELSBRIGHTNESSCOMPARATOR_H

#include <qstring.h>

#include <domain/interfaces/business/icomparator.h>

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct PixelsBrightnessComparisonResult {
    QString firstImageName;
    QString secondImageName;

    int totalPixels;                // Total number of pixels

    int sameColorCount;             // Number of pixels with the same color
    double sameColorPercent;        // Percentage of pixels with the same color

    int brighterCount;              // Number of pixels brighter in the first image
    double brighterPercent;         // Percentage of brighter pixels

    int darkerCount;                // Number of pixels darker in the first image
    double darkerPercent;           // Percentage of darker pixels

    int firstImageTotalBrightness;
    int secondImageTotalBrightness;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// This class compares the brightness of corresponding pixels in two images

class PixelsBrightnessComparator : public IComparator
{
public:
    PixelsBrightnessComparator() = default;
    virtual ~PixelsBrightnessComparator() = default;

    // IComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    ComparisonResultVariantPtr compare(const ComparableImage &first,
                                       const ComparableImage &second) override;
    QString getFullName() const override;

private:
    PixelsBrightnessComparisonResult compareImages(const QImage &image1,
                                                   const QString &name1,
                                                   const QImage &image2,
                                                   const QString &name2
                                                   );

    QString formatResultToHtml(const PixelsBrightnessComparisonResult &result);

};

#endif // PIXELSBRIGHTNESSCOMPARATOR_H
