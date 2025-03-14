#ifndef FILTER_H
#define FILTER_H

#include "imageprocessor.h"

#include <qimage.h>
#include <qstring.h>

/*
 * A base class for all filters' interfaces in the app
 */

class IFilter : public IImageProcessor {
public:
    virtual QImage filter(QImage image) = 0;

    virtual QList<Property> getDefaultProperties() const override { return {}; }

    virtual void setProperties(QList<Property>) override { };

    virtual void reset() override { }

    ImageProcessorType getType() const override { return ImageProcessorType::Filter; }
};

#endif // FILTER_H
