#ifndef SHARPNESSCOMPARATOR_H
#define SHARPNESSCOMPARATOR_H

#include <domain/interfaces/comporator.h>


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

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;
    QString getFullName() const override;

private:
    double calculateSharpness(const QImage &image);
    QString formatResultToHtml(const SharpnessComparisonResult &result);
    SharpnessComparisonResult compareImages(QImage image1, QString name1, QImage image2, QString name2);
};

#endif // SHARPNESSCOMPARATOR_H
