#include "mathhelper.h"

#include <qstring.h>

RoundedResult MathHelper::roundAndCompare(double num1, double num2, int precision) {
    auto roundToPrecision = [](double value, int prec) -> double {
        double factor = std::pow(10.0, prec);
        return std::round(value * factor) / factor;
    };

    double rounded1 = roundToPrecision(num1, precision);
    double rounded2 = roundToPrecision(num2, precision);

    if (rounded1 == rounded2) {
        int extraPrecision = precision;
        while (true) {
            ++extraPrecision;
            rounded1 = roundToPrecision(num1, extraPrecision);
            rounded2 = roundToPrecision(num2, extraPrecision);

            if (rounded1 != rounded2) {
                break;
            }

            if (extraPrecision > 15) {
                extraPrecision = precision;
                break;
            }
        }

        QString result1 = QString::number(rounded1, 'f', extraPrecision);
        QString result2 = QString::number(rounded2, 'f', extraPrecision);

        return {result1, result2, rounded1, rounded2};
    }

    QString result1 = QString::number(rounded1, 'f', precision);
    QString result2 = QString::number(rounded2, 'f', precision);

    return {result1, result2, rounded1, rounded2};
}

