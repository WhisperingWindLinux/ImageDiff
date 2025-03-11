#ifndef ATRANSFORMER_H
#define ATRANSFORMER_H

#include "aimageprocessor.h"

#include <qimage.h>
#include <qstring.h>

/*
 * A base class for all transformers' interfaces in the app
 */

class ATransformer : public AImageProcessor {
public:
    virtual QImage transform(QImage image) = 0;

    virtual QList<Property> getDefaultProperties() const override { return {}; }

    virtual void setProperties(QList<Property>) override { };

    ImageProcessorType getType() const override { return ImageProcessorType::Transformer; }
};

#endif // ATRANSFORMER_H
