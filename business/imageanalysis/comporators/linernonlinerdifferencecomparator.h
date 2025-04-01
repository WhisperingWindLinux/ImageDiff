#ifndef LINERNONLINERDIFFERENCECOMPARATOR_H
#define LINERNONLINERDIFFERENCECOMPARATOR_H

#include <domain/interfaces/business/icomparator.h>

struct LinerNonLinerComparisonResult {
    LinerNonLinerComparisonResult(double meanDifference, double threshold,
                                  double stdDeviation)
        : meanDifference(meanDifference), threshold(threshold),
        stdDeviation(stdDeviation) {}

    double meanDifference;
    double threshold;
    double stdDeviation;
};

class LinerNonLinerDifferenceComparator : public IComparator
{
public:
    LinerNonLinerDifferenceComparator();
    virtual ~LinerNonLinerDifferenceComparator() = default;

    // IImageProcessor interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    QString getFullName() const override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property>) override;
    void reset() override;

    // IComparator interface

    ComparisonResultVariantPtr compare(const ComparableImage &first, const ComparableImage &second) override;

private:
    double threshold;

    LinerNonLinerComparisonResult compareImages(const QImage &image1,
                                                const QString &,
                                                const QImage &image2,
                                                const QString &
                                                );

     QString formatResultToHtml(const LinerNonLinerComparisonResult &result);
};

#endif // LINERNONLINERDIFFERENCECOMPARATOR_H
