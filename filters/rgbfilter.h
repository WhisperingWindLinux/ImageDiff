#ifndef RGBFILTER_H
#define RGBFILTER_H

#include <interfaces/filter.h>


class GenericRgbFilter : public IFilter
{
protected:
    enum class RgbChannel { R, G, B };

public:
    GenericRgbFilter(RgbChannel channel);
    virtual ~GenericRgbFilter() = default;

// ATransformer interface

public:
    QString name() override;
    QString hotkey() override;
    QString description() override;
    QImage filter(QImage image) override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property> properties) override;

private:
    RgbChannel channel;
    bool isOutputImageColored;

    static QImage extractChannel(const QImage& image, bool isGrayscale, RgbChannel channels);
};

class RedChannelFilter : public GenericRgbFilter {
public:
    RedChannelFilter()
        : GenericRgbFilter(RgbChannel::R) {}
    virtual ~RedChannelFilter() = default;
};

class GreenChannelFilter : public GenericRgbFilter {
public:
    GreenChannelFilter()
        : GenericRgbFilter(RgbChannel::G) {}
    virtual ~GreenChannelFilter() = default;
};

class BlueChannelFilter : public GenericRgbFilter {
public:
    BlueChannelFilter()
        : GenericRgbFilter(RgbChannel::B) {}
    virtual ~BlueChannelFilter() = default;
};


#endif // RGBFILTER_H
