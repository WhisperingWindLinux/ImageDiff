#ifndef COLORSSATURATIONCOMPORATOR_H
#define COLORSSATURATIONCOMPORATOR_H

#include <QString>
#include <QImage>
#include <QColor>

// Structure to hold the comparison result
struct ColorsSaturationComparisonResult {
    QString imagePath1;       // Full path to the first image
    QString imagePath2;       // Full path to the second image
    double avgSaturation1;    // Average saturation of the first image
    double avgSaturation2;    // Average saturation of the second image
    QString moreSaturated;    // Which image is more saturated ("Image 1", "Image 2", or "Equal")
};

class ColorsSaturationComporator {
public:
    // Constructor that accepts two image file paths
    ColorsSaturationComporator(const QString& path1, const QString& path2);

    // Function to compare the images and return the result
    ColorsSaturationComparisonResult compareImages();

    // Function to format the comparison result as an HTML string
    static QString formatResultToHtml(const ColorsSaturationComparisonResult& result);

private:
    QString imagePath1;
    QString imagePath2;

    // Helper function to calculate the average saturation of an image
    double calculateAverageSaturation(const QImage& image);
};


/*

Code Explanation

1. compareImages:
   - Loads the two images from the provided file path.
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
