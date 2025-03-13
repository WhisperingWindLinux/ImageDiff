#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <imageprocessorsmanager/property.h>

enum class ImageProcessorType { Comparator, Filter };

class AImageProcessor {
public:    
    // It must be unique; displayed in the menu.
    virtual QString name() = 0;

    // A hotkey is used for quickly calling the comparator.
    // All hotkeys must be unique; if this is not the case,
    // an error will occur when the application starts.
    // Example:  "O", "Ctrl+O", etc.
    virtual QString hotkey() = 0;

    virtual QString description() = 0;

    virtual ImageProcessorType getType() const = 0;

    virtual QList<Property> getDefaultProperties() const = 0;

    virtual void setProperties(QList<Property>) = 0;

    virtual void reset() = 0;
};

/*

The hotkeys table

-----------------------------------------------------------------------------------------------
|      a image processor name                                          |   a hotkey (one key) |
-----------------------------------------------------------------------------------------------

Switch between images being compared                                               S

Show the difference in pixel values as an image (Comparator)                       D

Show the absolute difference statistics for pixel values (Comparator)              V

Show pixels' brigthness difference statistics (Comparator)                         N

Show pixels' contrast difference statistics (Comparator)                           K

Show pixels' saturation difference statistics (Comparator)                         T

Dummy comparator (Comparator; For Testing Purposes Only)                           X

Color picker                                                                       P

Advanced Color Picker                                                              A

Show Original Image (Filter)                                                       C

Show Grayscale Image (Filter)                                                      L

Show Red channel only (Filter)                                                     R

Show Green channel only (Filter)                                                   G

Show Blue channel only (Filter)                                                    B

Sharpness comporator                                                               H

Compare the proximity of two images to an original image                           Y                                                                    F


*/

#endif // IMAGEPROCESSOR_H
