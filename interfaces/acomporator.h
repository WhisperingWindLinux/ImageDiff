#ifndef ACOMPORATOR_H
#define ACOMPORATOR_H

#include "comparisionresultvariant.h"
#include <qstring.h>

enum class ComporatorContentType { Image, Video };

/*
 * A base class for all comparators' interfaces in the app
 */

class AComparator {
public:
    // It must be unique; displayed in the menu.
    virtual QString name() = 0;

    // A hotkey is used for quickly calling the comparator.
    // All hotkeys must be unique; if this is not the case,
    // an error will occur when the application starts.
    // Example:  "O", "Ctrl+O", etc.
    virtual QString hotkey() = 0;

    virtual QString description() = 0;

    virtual std::shared_ptr<ComparisionResultVariant> compare(QList<QString> filesPath) = 0;

    virtual ComporatorContentType contentType() = 0;
};

#endif // ACOMPORATOR_H

/*

The hotkeys table

-----------------------------------------------------------------------------------
|      a comporator name                                            |   a hotkey  |
-----------------------------------------------------------------------------------

Switch between images being compared                                       S

Show the difference in pixel values as an image                            D

Show the absolute difference statistics for pixel values                   A

Show pixels' brigthness difference statistics                              B

Show pixels' contrast difference statistics                                K

Show pixels' saturation difference statistics                              T

Color picker                                                               P

*/
