#ifndef PIXELSBRIGHTNESSCOMPARATOR_H
#define PIXELSBRIGHTNESSCOMPARATOR_H

#include <qstring.h>

#include <interfaces/acomporator.h>

// Structure to store comparison results
struct ComparisonResult {
    QString firstImagePath; // The image used as the basis for comparison
    QString secondImagePath;

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


class PixelsBrightnessComparator : public AComparator
{
public:

    // AComparator interface

    QString name();
    QString hotkey();
    QString description();
    std::shared_ptr<ComparisionResultVariant> compare(QList<QString> filesPath);
    virtual ComporatorContentType contentType();

private:
    ComparisonResult compareImages(const QString imagePath1, const QString imagePath2);
    QString formatResultToHtml(const ComparisonResult& result);

};

#endif // PIXELSBRIGHTNESSCOMPARATOR_H
