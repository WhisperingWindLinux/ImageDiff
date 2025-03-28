#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <QString>

struct RoundedResult {
    QString string1;
    QString string2;
    double value1;
    double value2;
};


class MathHelper
{
public:
    MathHelper() = delete;
    ~MathHelper() = delete;

    static RoundedResult roundAndCompare(double num1, double num2, int precision);
};

#endif // MATHHELPER_H
