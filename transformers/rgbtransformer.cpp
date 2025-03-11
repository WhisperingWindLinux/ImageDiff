#include "rgbtransformer.h"

GenericRgbTransformer::GenericRgbTransformer(RgbChannel channel)
    : channel(channel),
      isOutputImageColored(true)
{

}

QString GenericRgbTransformer::name() {
    switch(channel) {
    case RgbChannel::R:
        return "Show Red channel";
    case RgbChannel::G:
        return "Show Green channel";
    case RgbChannel::B:
        return "Show Blue channel";
    }
    throw new std::runtime_error("Error: an incorrect RGB chnannel in GenericRgbTransformer.");
}

QString GenericRgbTransformer::hotkey() {
    switch(channel) {
    case RgbChannel::R:
        return "R";
    case RgbChannel::G:
        return "G";
    case RgbChannel::B:
        return "B";
    }
    throw new std::runtime_error("Error: an incorrect RGB chnannel in GenericRgbTransformer.");
}

QString GenericRgbTransformer::description() {
    QString channelName;
    if (channel == RgbChannel::R) {
        channelName = "Red";
    } else if (channel == RgbChannel::G) {
        channelName = "Green";
    } else if (channel == RgbChannel::B) {
        channelName = "Blue";
    } else {
        throw new std::runtime_error("Error: an incorrect RGB chnannel in GenericRgbTransformer.");
    }

    QString description = QString("Leaves only the %1 channel on the RGB image.").arg(channelName);
    return description;
}

QImage GenericRgbTransformer::transform(QImage image) {
    return extractChannel(image, isOutputImageColored, channel);
}

QImage GenericRgbTransformer::extractChannel(const QImage& image, bool isImageColored, RgbChannel channel) {

    QImage oneChannelImage(image.size(),
                           isImageColored ?
                               QImage::Format_ARGB32
                                       : QImage::Format_Grayscale8);

    // Iterate over each pixel
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            // Extract the original pixel
            QRgb pixel = image.pixel(x, y);

            if (channel == RgbChannel::R) {
                int red = qRed(pixel);
                QRgb newPixel = qRgba(red, 0, 0, qAlpha(pixel));
                oneChannelImage.setPixel(x, y, newPixel);
            } else if (channel == RgbChannel::G) {
                int green = qGreen(pixel);
                QRgb newPixel = qRgba(0, green, 0, qAlpha(pixel));
                oneChannelImage.setPixel(x, y, newPixel);
            } else if (channel == RgbChannel::B) {
                int blue = qBlue(pixel);
                QRgb newPixel = qRgba(0, 0, blue, qAlpha(pixel));
                oneChannelImage.setPixel(x, y, newPixel);
            } else {
                throw new std::runtime_error("Error: An incorrect RGB channel was requested.");
            }
        }
    }
    return oneChannelImage;
}

QList<Property> GenericRgbTransformer::getDefaultProperties() const {

    QList<QString> alternatives = { "Colored", "Grayscale" };

    QString description = "Represents the choice between colored and grayscale image in an R/G/B mode.";

    Property colorModeProperty = Property::createStringProperty("Color mode", description, alternatives, 0);

    QList<Property> properties = { colorModeProperty };

    return properties;

}

void GenericRgbTransformer::setProperties(QList<Property> properties) {
    if (properties.size() != 1) {
        return;
    }
    Property colorModeProperty = properties[0];

    if (colorModeProperty.getPoropertyName() != "Color mode"
        || colorModeProperty.getPropertyType() != Property::Type::Alternatives)
    {
        return;
    }
    int iColorModeProperty = colorModeProperty.getValue();
    isOutputImageColored = (iColorModeProperty == 0);
}
