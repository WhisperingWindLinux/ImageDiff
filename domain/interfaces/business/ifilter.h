#ifndef IFILTER_H
#define IFILTER_H

#include <domain/interfaces/business/imageprocessor.h>

#include <qimage.h>
#include <qstring.h>

/*
 * A base class for all filters' interfaces in the app
 */

class IFilter : public IImageProcessor {
public:
    virtual QImage filter(const QImage &image) = 0;

    virtual QList<Property> getDefaultProperties() const override;

    virtual void setProperties(QList<Property>) override;;

    ImageProcessorType getType() const override;
};

typedef std::shared_ptr<IFilter> IFilterPtr;

#endif // IFILTER_H
