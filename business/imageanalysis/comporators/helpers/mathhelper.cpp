#include "mathhelper.h"

#include <QtCore/qmath.h>
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

QString MathHelper::formatPercentageValue(double value, int precision) {
    if (std::abs(value) < std::numeric_limits<double>::epsilon()) {
        return "-";
    }
    double truncatedValue = roundOrTruncate(value, precision, false);
    if (std::abs(truncatedValue) < std::numeric_limits<double>::epsilon()) {
        double threshold = std::pow(10.0, -precision);
        return QString("&lt;%1%").arg(threshold);
    } else {
        return QString("%1%").arg(truncatedValue);
    }
}

BeautifiedPercentageResult MathHelper::calcAndBeautifyPercentageValue(double value1,
                                                                      double value2,
                                                                      QString stringIfValue1Greater,
                                                                      QString stringIfValue2Greater,
                                                                      QString stringIfValuesEqual,
                                                                      QString stringIfPersantageIsZero
                                                                     )
{
    QString comparisonResult, persantageResult;
    double percentage = 0;
    bool isEqually = false;
    if (value1 > value2) {
        comparisonResult = stringIfValue1Greater;
        percentage = ((value1 - value2) / value2) * 100.0;
        persantageResult = MathHelper::formatPercentageValue(percentage, 1);
    } else if (value2 > value1) {
        percentage = ((value2 - value1) / value1) * 100.0;
        comparisonResult = stringIfValue2Greater;
        persantageResult = MathHelper::formatPercentageValue(percentage, 1);
    } else {
        comparisonResult = stringIfValuesEqual;
        persantageResult = stringIfPersantageIsZero;
        isEqually = true;
    }
    return BeautifiedPercentageResult{comparisonResult, isEqually, persantageResult};
}

double MathHelper::roundOrTruncate(double value, int precision, bool round) {
    double factor = qPow(10.0, precision);
    if (round) {
        return qRound(value * factor) / factor;
    } else {
        return static_cast<double>(static_cast<qint64>(value * factor)) / factor;
    }
}


