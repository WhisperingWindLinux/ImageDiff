#ifndef FILTER_H
#define FILTER_H

#include <domain/interfaces/imageprocessor.h>

#include <qimage.h>
#include <qstring.h>

/*
 * A base class for all filters' interfaces in the app
 */

class IFilter : public IImageProcessor {
public:
    virtual QImage filter(const QImage &image) = 0;

    virtual QList<Property> getDefaultProperties() const override { return {}; }

    virtual void setProperties(QList<Property>) override { };

    ImageProcessorType getType() const override { return ImageProcessorType::Filter; }
};

typedef std::shared_ptr<IFilter> IFilterPtr;

#endif // FILTER_H
