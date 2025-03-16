#ifndef CONTRASTCOMPORATOR_H
#define CONTRASTCOMPORATOR_H

#include <qstring.h>
#include <qimage.h>

#include <domain/interfaces/comporator.h>

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct ContrastComparisonResult {
    QString image1Name;               // The Name of the first image
    QString image2Name;               // The Name of the second image
    double contrast1;            // Contrast of the first image
    double contrast2;            // Contrast of the second image
    QString moreContrastImageName;   // The name of the image with higher contrast
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// The algorithm calculates the contrast of an image based on the statistical
// analysis of pixel brightness (luminance).
// It compares two images based on this criterion.

class ContrastComporator : public IComparator
{
public:

    ContrastComporator() = default;
    virtual ~ContrastComporator() = default;

    // AComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;
    QString getFullName() const override;

private:
    ContrastComparisonResult compareImages(QImage image1,
                                           QString path1,
                                           QImage image2,
                                           QString path2
                                           );
    QString formatResultToHtml(const ContrastComparisonResult& result);
    static double calculateContrast(const QImage& image);
};




/*
 *
### How the algorithm works:
1. Luminance (Brightness):
   - For each pixel in the image, its luminance (brightness) is calculated using the formula:

     luminance = 0.2126 * red + 0.7152 * green + 0.0722 * blue

   This is a standard formula for brightness calculation in color spaces, considering the human eye's perception of colors
(green contributes the most, blue the least).

2. Mean Luminance:
   - The algorithm calculates the mean luminance of all pixels in the image:

     μ = Σ_(i=1)^N luminance_i / N

   where N is the total number of pixels.

3. Contrast (Variance and Standard Deviation):
   - Next, it calculates the variance of luminance (the spread of brightness values relative to the mean):

     σ^2 = Σ_(i=1)^N (luminance_i - μ)^2 / N

   - Based on the variance, it computes the standard deviation (square root of variance):

     σ = √(σ^2)

   The standard deviation (σ) is used as a measure of image contrast. The higher the value of σ, the greater the spread
of brightness among pixels, and hence, the higher the contrast.

4. Comparing Contrast:
   - The contrast of two images (σ_1 and σ_2) is compared to determine which image has higher contrast.

---

### Difference from Simple Brightness
If the algorithm compared only brightness, it would work with the mean luminance value (μ) without considering the
spread of values. In this case, it would only determine the overall "lighting" of an image, not its contrast.

Example:
- An image with a uniform gray background (all pixels have the same brightness) can be very bright but have zero contrast.
- An image with sharp transitions between light and dark areas will have high contrast, even if its mean brightness is low.

### Summary
The algorithm calculates the contrast of an image based on the statistical analysis of pixel brightness (luminance).
 It does not compare brightness directly but uses it to determine the spread of values, allowing for the assessment
of differences between light and dark areas of the image.

*/

#endif // CONTRASTCOMPORATOR_H
