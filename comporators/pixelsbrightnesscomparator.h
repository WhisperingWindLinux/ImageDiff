#ifndef PIXELSBRIGHTNESSCOMPARATOR_H
#define PIXELSBRIGHTNESSCOMPARATOR_H

#include <qstring.h>

#include <interfaces/comporator.h>

// Structure to store comparison results
struct PixelsBrightnessComparisonResult {

    QString name1;            // The name of the first image
    QString name2;            // The name of the second image

    int totalPixels;          // Total number of pixels

    int sameColorCount;       // Number of pixels with the same color
    double sameColorPercent;  // Percentage of pixels with the same color

    int brighterCount;        // Number of pixels brighter in the first image
    double brighterPercent;   // Percentage of brighter pixels

    int darkerCount;          // Number of pixels darker in the first image
    double darkerPercent;     // Percentage of darker pixels

    int totalBrightness1;
    int totalBrightness2;
};


class PixelsBrightnessComparator : public IComparator
{
public:
    PixelsBrightnessComparator() = default;
    virtual ~PixelsBrightnessComparator() = default;

    // AComparator interface

    QString name() override;
    QString hotkey() override;
    QString description() override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;

private:
    PixelsBrightnessComparisonResult compareImages(QImage image1,
                                                   QString name1,
                                                   QImage image2,
                                                   QString name2
                                                   );

    QString formatResultToHtml(const PixelsBrightnessComparisonResult& result);

};

#endif // PIXELSBRIGHTNESSCOMPARATOR_H
