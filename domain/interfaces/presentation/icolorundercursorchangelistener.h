#ifndef ICOLORUNDERCURSORCHANGELISTENER_H
#define ICOLORUNDERCURSORCHANGELISTENER_H

#include <domain/valueobjects/imagepixelcolor.h>

class IColorUnderCursorChangeListener {
public:
    virtual void onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                           const std::optional<ImagePixelColor> &hiddenImageRgbValue) = 0;
};

#endif // ICOLORUNDERCURSORCHANGELISTENER_H
