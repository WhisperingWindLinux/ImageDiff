#ifndef PIXELSABSOLUTEVALUECOMPARATOR_H
#define PIXELSABSOLUTEVALUECOMPARATOR_H

#include <QPair>

#include <interfaces/comporator.h>

// This class calculates the difference in RGB values between two images.
// It then displays the differences as a list of ranges along with the corresponding pixel counts.

class PixelsAbsoluteValueComparator : public IComparator
{
public:
    PixelsAbsoluteValueComparator() = default;
    virtual ~PixelsAbsoluteValueComparator() = default;

    // AComparator interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;
    QString getFullName() const override;

private:
    QMap<QPair<int, int>, QPair<int, double>> compareImages(QImage image1, QImage image2);

    QString formatResultToHtml(const QMap<QPair<int, int>, QPair<int, double>>& result);
};

#endif // PIXELSABSOLUTEVALUECOMPARATOR_H
