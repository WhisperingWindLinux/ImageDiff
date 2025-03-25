#ifndef PIXELSASOLUTVALUEHELPER_H
#define PIXELSASOLUTVALUEHELPER_H

#include <map>
#include <qimage.h>
#include <domain/valueobjects/pixeldiffrencerange.h>

enum class PixelsDifferenceCalculationMode { DifferenceBySingleLargestComponent, DifferenceByAllComponents };

class PixelsAbsolutValueHelper
{
public:
    PixelsAbsolutValueHelper(PixelsDifferenceCalculationMode mode);

    QList<PixelDifferenceRange> generateDifferenceStringResult(const QImage &image1, const QImage &image2);
    QImage generateDifferenceImage(const QImage &image1, const QImage &image2);
    static QString getColorRangeDescription();
    
    QImage generateDifferenceImageByCustomRage(const QImage &image1,
                                               const QImage &image2,
                                               int startOfRange,
                                               int endOfRange);
private:
    PixelsDifferenceCalculationMode currentMode;

    int calculateDiff(QColor color1, QColor color2);
    std::map<int, QColor> generateColorMap(const QList<PixelDifferenceRange> &ranges);
};

#endif // PIXELSASOLUTVALUEHELPER_H
