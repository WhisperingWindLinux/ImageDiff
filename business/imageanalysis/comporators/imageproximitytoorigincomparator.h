#ifndef IMAGEPROXIMITYTOORIGINCOMPARATOR_H
#define IMAGEPROXIMITYTOORIGINCOMPARATOR_H

#include <domain/interfaces/comparator.h>


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct ImageProximityToOriginResult {
    ImageProximityToOriginResult(QString image1Name, QString image2Name,
                                 qint64 totalDifference1, qint64 totalDifference2,
                                 QString resultDescription)
        : image1Name(std::move(image1Name)), image2Name(std::move(image2Name)),
        totalDifference1(totalDifference1), totalDifference2(totalDifference2),
        resultDescription(std::move(resultDescription)) {}
    QString image1Name;
    QString image2Name;
    qint64 totalDifference1;
    qint64 totalDifference2;
    QString resultDescription;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// This class calculates the total difference between two
// images using the squares of the differences in color
// component values (R, G, B) of each pixel.
// According to this criterion, the closeness
// of each of the two images to the original is evaluated.

class ImageProximityToOriginComparator : public IComparator
{
public:
    ImageProximityToOriginComparator() = default;
    virtual ~ImageProximityToOriginComparator() = default;

    // AComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    ComparisonResultVariantPtr compare(const ComparableImage &first,
                                       const ComparableImage &second) override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property> properties) override;
    void reset() override;
    QString getFullName() const override;
    bool isPartOfAutoReportingToolbox() override;

private:
    QString pathToOriginalImage;
    QImage originalImage;

    double calculateSharpness(const QImage &image);

    qint64 calculateTotalDifference(const QImage &image, const QImage &originalImage);

    ImageProximityToOriginResult compareImages(const QImage &image1,
                                               const QImage &image2,
                                               const QString &name1,
                                               const QString &name2,
                                               const QImage &originalImage
                                               );

    QString formatResultToHtml(const ImageProximityToOriginResult &result);
};

#endif // IMAGEPROXIMITYTOORIGINCOMPARATOR_H
