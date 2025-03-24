#ifndef PIXELSABSOLUTEVALUEFORMATTER_H
#define PIXELSABSOLUTEVALUEFORMATTER_H

#include <QString>

#include <domain/valueobjects/pixeldiffrencerange.h>

#include <business/imageanalysis/comporators/pixelsabsolutevaluecomparator.h>

class PixelsAbsolutValueFormatter
{
public:
    PixelsAbsolutValueFormatter() = delete;
    ~PixelsAbsolutValueFormatter() = delete;

    static QString formatResultToHtml(QList<PixelDifferenceRange> &result,
                                      QString comporatorFullName,
                                      PixelsAbsoluteValueComparator::Mode currentMode
                                      );

};

#endif // PIXELSABSOLUTEVALUEFORMATTER_H
