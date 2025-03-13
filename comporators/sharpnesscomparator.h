#ifndef SHARPNESSCOMPARATOR_H
#define SHARPNESSCOMPARATOR_H

#include <interfaces/comporator.h>

struct SharpnessComparisonResult {

    QString name1;
    QString name2;

    double sharpness1;
    double sharpness2;

    QString sharperImage;
};


class SharpnessComparator : public AComparator
{
public:
    SharpnessComparator() = default;
    QString name();
    QString hotkey();
    QString description();
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second);
private:
    double calculateSharpness(const QImage &image);
    QString formatResultToHtml(const SharpnessComparisonResult &result);
    SharpnessComparisonResult compareImages(QImage image1, QString name1, QImage image2, QString name2);
};

/*
### Explanation:
1. Method calculateSharpness:
   - Uses pixel intensity gradients to assess sharpness.
   - Calculates the difference in brightness between the current pixel and its neighboring pixels to the right and below.
   - Sums up the gradient magnitudes to obtain an overall sharpness score.

2. Method compareImages:
   - Calculates the sharpness for two images.
   - Compares the values and determines which image is sharper.

3. Method formatResultToHtml:
   - Creates an HTML table with the comparison results.

4. Interface:
   - Compatible with your current interface.
   - Returns a string with the results formatted in HTML.
*/

#endif // SHARPNESSCOMPARATOR_H
