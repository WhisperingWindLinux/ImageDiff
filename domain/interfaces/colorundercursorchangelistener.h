#ifndef COLORUNDERCURSORCHANGELISTENER_H
#define COLORUNDERCURSORCHANGELISTENER_H

#include <domain/valueobjects/imagepixelcolor.h>

class IColorUnderCursorChangeListener {
public:
    virtual void onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                           const ImagePixelColor &hiddenImageRgbValue) = 0;
};

#endif // COLORUNDERCURSORCHANGELISTENER_H
