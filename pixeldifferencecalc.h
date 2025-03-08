#ifndef PIXELDIFFERENCECALC_H
#define PIXELDIFFERENCECALC_H

#include <QPair>


class PixelDifferenceCalc
{
private:
    PixelDifferenceCalc();

public:
    static QMap<QPair<int, int>, QPair<int, double> > calculatePixelDifferences(const QString &image1Path, const QString &image2Path);
};

#endif // PIXELDIFFERENCECALC_H
