#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <QString>

struct RoundedResult {
    QString string1;
    QString string2;
    double value1;
    double value2;
};

struct BeautifiedPercentageResult {
    QString resultDescription;
    bool isEqually;
    QString percentageResult;
};

class MathHelper
{
public:
    MathHelper() = delete;
    ~MathHelper() = delete;

    static QString formatPercentageValue(double value, int precision = 1);

    static BeautifiedPercentageResult calcAndBeautifyPercentageValue(double value1,
                                                                     double value2,
                                                                     QString stringIfValue1Greater,
                                                                     QString stringIfValue2Greater,
                                                                     QString stringIfValuesEqual,
                                                                     QString stringIfPersantageIsZero = ""
                                                                     );

    static RoundedResult roundAndCompare(double num1, double num2, int precision = 1);

private:
    static double roundOrTruncate(double value, int precision, bool round);

};

#endif // MATHHELPER_H
