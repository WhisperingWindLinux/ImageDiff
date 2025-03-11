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
    html += "<html><head><style>";
    html += "table { border-collapse: collapse; width: 100%; }";
    html += "th, td { border: 1px solid black; padding: 8px; text-align: left; }";
    html += "th { background-color: #f2f2f2; }";
    html += "</style></head><body>";
    html += "<h3>Image Comparison Results</h3><br>";
    html += "<table>";
    html += "<tr><th>Parameter</th><th>Image 1</th><th>Image 2</th></tr>";
    html += QString("<tr><td>Filename</td><td>%1</td><td>%2</td></tr>").arg(result.name1, result.name2);
    html += QString("<tr><td>Average Saturation</td><td>%1</td><td>%2</td></tr>")
                .arg(result.avgSaturation1, 0, 'f', 4)
                .arg(result.avgSaturation2, 0, 'f', 4);
    html += QString("<tr><td>More Saturated Image</td><td colspan='2'>%1</td></tr>")
                .arg(result.moreSaturatedImage);
    html += "</table>";
    html += "</body></html>";

    return html;
}

QString ColorsSaturationComporator::name() {
    return "Show pixels' saturation difference statistics";
}

QString ColorsSaturationComporator::hotkey() {
    return "T";
}

QString ColorsSaturationComporator::description() {
    return QString("This program loads images, calculates their average saturation based on ")
    + "pixel HSV values, compares the results, and formats the comparison as "
        + "an HTML table.";
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
