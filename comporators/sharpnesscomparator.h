#ifndef SHARPNESSCOMPARATOR_H
#define SHARPNESSCOMPARATOR_H

#include <interfaces/comporator.h>

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct SharpnessComparisonResult {

    QString name1;
    QString name2;

    double sharpness1;
    double sharpness2;

    QString sharperImage;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// This class compares the sharpness of two images by calculating
// their sharpness values based on the gradient magnitude of pixel intensity
// differences in both the horizontal and vertical directions.

class SharpnessComparator : public IComparator
{
public:
    SharpnessComparator() = default;
    virtual ~SharpnessComparator() = default;

    QString name() const override;
    QString hotkey() const override;
    QString htmlFormattedHelp() const override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second);
private:
    double calculateSharpness(const QImage &image);
    QString formatResultToHtml(const SharpnessComparisonResult &result);
    SharpnessComparisonResult compareImages(QImage image1, QString name1, QImage image2, QString name2);
};

#endif // SHARPNESSCOMPARATOR_H
