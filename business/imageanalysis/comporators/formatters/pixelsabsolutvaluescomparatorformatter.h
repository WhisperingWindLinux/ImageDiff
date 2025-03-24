#ifndef PIXELSABSOLUTVALUESCOMPARATORFORMATTER_H
#define PIXELSABSOLUTVALUESCOMPARATORFORMATTER_H

#include <QString>

#include <domain/valueobjects/pixeldiffrencerange.h>
#include <business/imageanalysis/comporators/pixelsabsolutevaluecomparator.h>

class PixelsAbsolutValuesComparatorFormatter
{
public:
    PixelsAbsolutValuesComparatorFormatter() = delete;
    ~PixelsAbsolutValuesComparatorFormatter() = delete;

    static QString formatResultToHtml(QList<PixelDifferenceRange> &result,
                                      QString comporatorFullName,
                                      PixelsAbsoluteValueComparator::Mode currentMode
                                      );

};

#endif // PIXELSABSOLUTVALUESCOMPARATORFORMATTER_H
