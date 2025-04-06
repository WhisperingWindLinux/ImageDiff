#ifndef IMAGEPIXELCOLOR_H
#define IMAGEPIXELCOLOR_H

#include <qstring.h>


class ImagePixelColor {
public:
    ImagePixelColor() {}

    ImagePixelColor(const ImagePixelColor &src) = default;

    ImagePixelColor(ImagePixelColor&&) = default;

    ImagePixelColor(const QString &imageName, int r, int g, int b)
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

    ImagePixelColor& operator=(const ImagePixelColor&) = default;

    ImagePixelColor& operator=(ImagePixelColor&&) = default;

private:
    QString imageName;
    int r;
    int g;
    int b;
};

#endif // IMAGEPIXELCOLOR_H











