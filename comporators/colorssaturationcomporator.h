#ifndef COLORSSATURATIONCOMPORATOR_H
#define COLORSSATURATIONCOMPORATOR_H

#include <QString>
#include <QImage>
#include <QColor>

#include <interfaces/comporator.h>

// Structure to hold the comparison result
struct ColorsSaturationComparisonResult {
    QString name1;              // Name of the first image
    QString name2;              // Name of the second image
    double avgSaturation1;      // Average saturation of the first image
    double avgSaturation2;      // Average saturation of the second image
    QString moreSaturatedImage; // The name of the image with higher saturation
};

class ColorsSaturationComporator : public AComparator {
public:

    // AComparator interface

    QString name() override;
    QString hotkey() override;
    QString description() override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;

private:
    // Function to compare the images and return the result
    ColorsSaturationComparisonResult compareImages(QImage image1,
                                                   QString name1,
                                                   QImage image2,
                                                   QString name2);

    // Function to format the comparison result as an HTML string
    static QString formatResultToHtml(const ColorsSaturationComparisonResult& result);

    // Helper function to calculate the average saturation of an image
    double calculateAverageSaturation(const QImage& image);
};


/*

Code Explanation

1. compareImages:
   - Receives two images being comapared.
   - Calculates their average saturation using calculateAverageSaturation.
   - Determines which image has higher saturation or if they are equal.

2.  calculateAverageSaturation:
   - Iterates through each pixel of the image.
   - Converts each pixel's color to HSV and extracts its saturation component.
   - Computes the average saturation.

3. formatResultAsHtml:
   - Formats the comparison results into an HTML table.
   - If both images have identical filenames, it uses their full paths for clarity.

*/


#endif // COLORSSATURATIONCOMPORATOR_H
