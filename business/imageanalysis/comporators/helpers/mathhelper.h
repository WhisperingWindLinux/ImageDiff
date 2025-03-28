#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <QString>

struct RoundedResult {
    QString string1;
    QString string2;
    double value1;
    double value2;
};

struct ExtendedRoundedResult {
    QString string1;
    QString string2;
    QString string3;
    double value1;
    double value2;
    double value3;
};

class MathHelper
{
public:
    MathHelper() = delete;
    ~MathHelper() = delete;

    static RoundedResult roundAndCompare(double num1, double num2, int precision);
    static ExtendedRoundedResult extendedRoundAndCompare(double num1, double num2, double num3, int precision);
};

#endif // MATHHELPER_H
