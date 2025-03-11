#ifndef RGBTRANSFORMER_H
#define RGBTRANSFORMER_H

#include <interfaces/ATransformer.h>


class GenericRgbTransformer : public ATransformer
{
protected:
    enum class RgbChannel { R, G, B };

public:
    GenericRgbTransformer(RgbChannel channel);

// ATransformer interface

public:
    QString name() override;
    QString hotkey() override;
    QString description() override;
    QImage transform(QImage image) override;
    QList<Property> getDefaultProperties() const override;
    void setProperties(QList<Property> properties) override;

private:
    RgbChannel channel;
    bool isOutputImageColored;

    static QImage extractChannel(const QImage& image, bool isGrayscale, RgbChannel channels);
};

class RedChannelTransformer : public GenericRgbTransformer {
public:
    RedChannelTransformer()
        : GenericRgbTransformer(RgbChannel::R) {}
};

class GreenChannelTransformer : public GenericRgbTransformer {
public:
    GreenChannelTransformer()
        : GenericRgbTransformer(RgbChannel::G) {}
};

class BlueChannelTransformer : public GenericRgbTransformer {
public:
    BlueChannelTransformer()
        : GenericRgbTransformer(RgbChannel::B) {}
};


#endif // RGBTRANSFORMER_H
