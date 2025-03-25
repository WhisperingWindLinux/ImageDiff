#ifndef PIXELSASOLUTVALUEHELPER_H
#define PIXELSASOLUTVALUEHELPER_H

#include <map>
#include <business/imageanalysis/comporators/pixelsabsolutevaluecomparator.h>
#include <domain/valueobjects/pixeldiffrencerange.h>


class PixelsAbsolutValueHelper
{
public:
    PixelsAbsolutValueHelper(PixelsAbsoluteValueComparator::Mode mode);

    QList<PixelDifferenceRange> generateDifferenceStringResult(const QImage &image1, const QImage &image2);
    QImage generateDifferenceImage(const QImage &image1, const QImage &image2);
    static QString getColorRangeDescription();

private:
    PixelsAbsoluteValueComparator::Mode currentMode;

    int calculateDiff(QColor color1, QColor color2);
    std::map<int, QColor> generateColorMap(const QList<PixelDifferenceRange> &ranges);
};

#endif // PIXELSASOLUTVALUEHELPER_H
