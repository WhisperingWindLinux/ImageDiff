#ifndef SHARPNESSCOMPARATOR_H
#define SHARPNESSCOMPARATOR_H

#include <domain/interfaces/business/icomparator.h>


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

    // AComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    ComparisonResultVariantPtr compare(const ComparableImage &first,
                                       const ComparableImage &second) override;
    QString getFullName() const override;

private:
    double calculateSharpness(const QImage& image);
    QString formatResultToHtml(const SharpnessComparisonResult &result);
    SharpnessComparisonResult compareImages(const QImage &image1,
                                            const QString &name1,
                                            const QImage &image2,
                                            const QString &name2);
};

#endif // SHARPNESSCOMPARATOR_H
