#ifndef RGBVALUE_H
#define RGBVALUE_H

#include <qstring.h>


class RgbValue {
public:
    RgbValue() {}

    RgbValue(const RgbValue &src) = default;

    RgbValue(RgbValue&&) = default;

    RgbValue(const QString &imageName, int r, int g, int b)
        : imageName(imageName),
          r(r),
          g(g),
          b(b) {}

    QString getImageName() const {
        return imageName;
    }

    int getR() const {
        return r;
    }

    int getG() const {
        return g;
    }

    int getB() const {
        return b;
    }

    RgbValue& operator=(const RgbValue&) = default;

    RgbValue& operator=(RgbValue&&) = default;

private:
    QString imageName;
    int r;
    int g;
    int b;
};

#endif // RGBVALUE_H











