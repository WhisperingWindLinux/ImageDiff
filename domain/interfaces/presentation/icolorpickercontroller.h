#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <domain/valueobjects/imagepixelcolor.h>


class IColorPickerController {
public:

    virtual ~IColorPickerController() = default;

    virtual void openColorPickerDialog() = 0;
    virtual void placeColorPickerToRightSideOfMainWindow() = 0;
    virtual void placeColorPickerToLeftSideOfMainWindow() = 0;

    virtual void onImagesClosed() = 0;
    virtual void onImagesOpened() = 0;

    virtual void onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                   const std::optional<ImagePixelColor> &hiddenImageRgbValue
                                   ) = 0;
};

#endif // COLORPICKER_H
