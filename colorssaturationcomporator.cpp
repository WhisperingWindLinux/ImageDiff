#include "colorssaturationcomporator.h"

#include <QFileInfo>
#include <QDebug>

ColorsSaturationComporator::ColorsSaturationComporator(const QString& path1, const QString& path2)
    : imagePath1(path1), imagePath2(path2) {}

// Compare the two images and return a structure with the results
ColorsSaturationComparisonResult ColorsSaturationComporator::compareImages() {
    QImage image1(imagePath1);
    QImage image2(imagePath2);

    if (image1.isNull() || image2.isNull()) {
        qWarning() << "Error: Unable to load one or both images.";
        return {};
    }

    double avgSat1 = calculateAverageSaturation(image1);
    double avgSat2 = calculateAverageSaturation(image2);

    QString moreSaturated;
    if (avgSat1 > avgSat2) {
        moreSaturated = "Image 1";
    } else if (avgSat2 > avgSat1) {
        moreSaturated = "Image 2";
    } else {
        moreSaturated = "Equal";
    }

    return {imagePath1, imagePath2, avgSat1, avgSat2, moreSaturated};
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
    QFileInfo fileInfo1(result.imagePath1);
    QFileInfo fileInfo2(result.imagePath2);

    QString name1 = fileInfo1.fileName();
    QString name2 = fileInfo2.fileName();

    // If filenames are identical, use full paths for clarity
    if (name1 == name2) {
        name1 = result.imagePath1;
        name2 = result.imagePath2;
    }

    QString html;
    html += "<html><head><style>";
    html += "table { border-collapse: collapse; width: 100%; }";
    html += "th, td { border: 1px solid black; padding: 8px; text-align: left; }";
    html += "th { background-color: #f2f2f2; }";
    html += "</style></head><body>";
    html += "<h3>Image Comparison Results</h3><br>";
    html += "<table>";
    html += "<tr><th>Parameter</th><th>Image 1</th><th>Image 2</th></tr>";
    html += QString("<tr><td>Filename</td><td>%1</td><td>%2</td></tr>").arg(name1, name2);
    html += QString("<tr><td>Average Saturation</td><td>%1</td><td>%2</td></tr>")
                .arg(result.avgSaturation1, 0, 'f', 4)
                .arg(result.avgSaturation2, 0, 'f', 4);
    html += QString("<tr><td>More Saturated Image</td><td colspan='2'>%1</td></tr>")
                .arg(result.moreSaturated);
    html += "</table>";
    html += "</body></html>";

    return html;
}
