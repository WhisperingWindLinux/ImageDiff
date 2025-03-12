#ifndef AIMAGEPROCESSOR_H
#define AIMAGEPROCESSOR_H

#include <imageprocessorsmanager/property.h>

enum class ImageProcessorType { Comparator, Transformer };

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
};

/*

The hotkeys table

--------------------------------------------------------------------------------------
|      a comporator name                                               |   a hotkey  |
--------------------------------------------------------------------------------------

Switch between images being compared                                           S

Show the difference in pixel values as an image (Comparator)                   D

Show the absolute difference statistics for pixel values (Comparator)          V

Show pixels' brigthness difference statistics (Comparator)                     N

Show pixels' contrast difference statistics (Comparator)                       K

Show pixels' saturation difference statistics (Comparator)                     T

Dummy comparator (Comparator; For Testing Purposes Only)                       Z

Color picker                                                                   P

Advanced Color Picker                                                          A

Show Original Image (Transformer)                                              C

Show Grayscale Image (Transformer)                                             L

Show Red channel only (Transformer)                                            R

Show Green channel only (Transformer)                                          G

Show Blue channel only (Transformer)                                           B


*/

#endif // AIMAGEPROCESSOR_H
