#ifndef RGBVALUE_H
#define RGBVALUE_H

#include <qstring.h>


class RgbValue {
public:
    RgbValue(const QString &imageName, int r, int g, int b) : imageName(imageName),
        r(r),
        g(g),
        b(b)
    {}

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

private:
    QString imageName;
    int r;
    int g;
    int b;
};

#endif // RGBVALUE_H











