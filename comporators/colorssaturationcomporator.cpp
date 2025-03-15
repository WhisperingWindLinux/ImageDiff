#include "colorssaturationcomporator.h"

#include <QFileInfo>
#include <QDebug>

// Compare the two images and return a structure with the results
ColorsSaturationComparisonResult ColorsSaturationComporator::compareImages(QImage image1,
                                                                           QString name1,
                                                                           QImage image2,
                                                                           QString name2
                                                                           )
{

    if (image1.isNull() || image2.isNull()) {
        qWarning() << "Error: Unable to load one or both images.";
        return {};
    }

    double avgSat1 = calculateAverageSaturation(image1);
    double avgSat2 = calculateAverageSaturation(image2);

    QString moreSaturated;
    if (avgSat1 > avgSat2) {
        moreSaturated = name1;
    } else if (avgSat2 > avgSat1) {
        moreSaturated = name2;
    } else {
        moreSaturated = "Equal";
    }

    return {name1, name2, avgSat1, avgSat2, moreSaturated};
}

// Calculate the average saturation of an image
double ColorsSaturationComporator::calculateAverageSaturation(const QImage& image) {
    double totalSaturation = 0.0;
    int pixelCount = 0;

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color(image.pixel(x, y));
            totalSaturation += color.toHsv().saturationF();  // Saturation in range [0.0, 1.0]
            ++pixelCount;
        }
    }

    return (pixelCount > 0) ? (totalSaturation / pixelCount) : 0.0;
}

// Format the comparison result as an HTML string
QString ColorsSaturationComporator::formatResultToHtml(const ColorsSaturationComparisonResult& result) {
    QString html;
    html += "<h2>Comparison of Image Saturation</h2>";
    html += QString("The coefficient of %1 is %2.")
                .arg(result.image1Name)
                .arg(result.avgSaturation1);
    html += "<br/>";
    html += QString("The coefficient of %1 is %2.")
                .arg(result.image2Name)
                .arg(result.avgSaturation2);
    html += "<br/><br/>";
    html += QString("<font color=\"green\">The more saturated image is %1.</font>")
                .arg(result.moreSaturatedImageName);
    html += "<br /><br />";
    html += QString("The range of coefficient values is [0.0, 1.0]. ")
            + "Where higher values indicate a more saturated image.";

    return html;
}

QString ColorsSaturationComporator::name() const {
    return "Saturation";
}

QString ColorsSaturationComporator::hotkey() const {
    return "T";
}

QString ColorsSaturationComporator::htmlFormattedHelp() const {
    return QString("This algorithm calculates average saturation based on pixel HSV values. ")
                   + "The range of claculated coefficient values is [0.0, 1.0]."
                   + " Where higher values indicate a more saturated image.";
}

std::shared_ptr<ComparisonResultVariant> ColorsSaturationComporator::compare(ComparableImage first, ComparableImage second) {
    auto result = compareImages(first.getImage(),
                                first.getName(),
                                second.getImage(),
                                second.getName()
                                );

    QString html = ColorsSaturationComporator::formatResultToHtml(result);
    return std::make_shared<ComparisonResultVariant>(html);
}
