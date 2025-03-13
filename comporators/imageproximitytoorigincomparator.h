#ifndef IMAGEPROXIMITYTOORIGINCOMPARATOR_H
#define IMAGEPROXIMITYTOORIGINCOMPARATOR_H

#include <interfaces/comporator.h>

// Class to compare the proximity of two images to an original image

struct ImageProximityToOriginResult {
    ImageProximityToOriginResult(QString image1Name, QString image2Name,
                                 int totalDifference1, int totalDifference2,
                                 QString resultDescription)
        : image1Name(std::move(image1Name)), image2Name(std::move(image2Name)),
        totalDifference1(totalDifference1), totalDifference2(totalDifference2),
        resultDescription(std::move(resultDescription)) {}
    QString image1Name;
    QString image2Name;
    int totalDifference1;
    int totalDifference2;
    QString resultDescription;
};

class ImageProximityToOriginComparator : public AComparator
{
public:
    ImageProximityToOriginComparator() = default;
    QString name() override;
    QString hotkey() override;
    QString description() override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property>) override;
    void reset() override;

private:
    QString pathToOriginalImage;

    double calculateSharpness(const QImage &image);
    int calculateTotalDifference(const QImage &image, const QImage &originalImage);
    ImageProximityToOriginResult compareImages(QImage image1,
                                               QImage image2,
                                               QString name1,
                                               QString name2,
                                               QImage originalImage
                                               );
    QString formatResultToHtml(const ImageProximityToOriginResult &result);
};

#endif // IMAGEPROXIMITYTOORIGINCOMPARATOR_H
