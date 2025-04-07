#ifndef IMAGEPIXELCOLOR_H
#define IMAGEPIXELCOLOR_H

#include <qstring.h>


class ImagePixelColor {
public:
    ImagePixelColor() {}

    ImagePixelColor(const ImagePixelColor &src) = default;

    ImagePixelColor(ImagePixelColor&&) = default;

    ImagePixelColor(const QString &imageName, int r, int g, int b)
        : mImageName(imageName),
          mR(r),
          mG(g),
          mB(b) {}

    QString getImageName() const {
        return mImageName;
    }

    int getR() const {
        return mR;
    }

    int getG() const {
        return mG;
    }

    int getB() const {
        return mB;
    }

    ImagePixelColor& operator=(const ImagePixelColor&) = default;
    ImagePixelColor& operator=(ImagePixelColor&&) = default;

private:
    QString mImageName;
    int mR;
    int mG;
    int mB;
};

#endif // IMAGEPIXELCOLOR_H











