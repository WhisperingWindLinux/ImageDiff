#ifndef COLORSSATURATIONCOMPORATOR_H
#define COLORSSATURATIONCOMPORATOR_H

#include <QString>
#include <QImage>
#include <QColor>

#include <interfaces/comporator.h>

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// Structure to hold the comparison result
struct ColorsSaturationComparisonResult {
    QString image1Name;                   // Name of the first image
    QString image2Name;                   // Name of the second image
    double avgSaturation1;                // Average saturation of the first image
    double avgSaturation2;                // Average saturation of the second image
    QString moreSaturatedImageName;       // The name of the image with higher saturation
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// This class calculates the average saturation based on pixel HSV values.
// It compares two images based on this criterion.

class ColorsSaturationComporator : public IComparator {
public:

    ColorsSaturationComporator() = default;
    virtual ~ColorsSaturationComporator() = default;

    // AComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    QString getFullName() const override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;

private:
    ColorsSaturationComparisonResult compareImages(QImage image1,
                                                   QString name1,
                                                   QImage image2,
                                                   QString name2);

    QString formatResultToHtml(const ColorsSaturationComparisonResult& result);

    // Helper function to calculate the average saturation of an image
    double calculateAverageSaturation(const QImage& image);
};

#endif // COLORSSATURATIONCOMPORATOR_H
