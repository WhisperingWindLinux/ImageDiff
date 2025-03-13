#ifndef PIXELSABSOLUTEVALUECOMPARATOR_H
#define PIXELSABSOLUTEVALUECOMPARATOR_H

#include <QPair>

#include <interfaces/comporator.h>

class PixelsAbsoluteValueComparator : public AComparator
{
public:

    // AComparator interface

    QString name() override;
    QString hotkey() override;
    QString description() override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;

private:
    // Function to calculate pixel differences between two images
    QMap<QPair<int, int>, QPair<int, double>> compareImages(QImage image1, QImage image2);

    // Function to format the comparison result as an HTML string
    static QString formatResultToHtml(const QMap<QPair<int, int>, QPair<int, double>>& result);
};

#endif // PIXELSABSOLUTEVALUECOMPARATOR_H
