#include "sharpnesscomparator.h"

#include "sharpnesscomparator.h"

#include <QFileInfo>
#include <QDebug>
#include <QtMath>

// Compare the two images and return a structure with the results
SharpnessComparisonResult SharpnessComparator::compareImages(QImage image1,
                                                             QString name1,
                                                             QImage image2,
                                                             QString name2)
{
    if (image1.isNull() || image2.isNull()) {
        qWarning() << "Error: Unable to load one or both images.";
        return {};
    }

    double sharpness1 = calculateSharpness(image1);
    double sharpness2 = calculateSharpness(image2);

    QString sharperImage;
    if (sharpness1 > sharpness2) {
        sharperImage = name1;
    } else if (sharpness2 > sharpness1) {
        sharperImage = name2;
    } else {
        sharperImage = "Equal";
    }

    return {name1, name2, sharpness1, sharpness2, sharperImage};
}

// Calculate the sharpness of an image using gradient magnitude
double SharpnessComparator::calculateSharpness(const QImage& image) {
    double totalGradient = 0.0;
    int pixelCount = 0;

    for (int y = 1; y < image.height() - 1; ++y) {
        for (int x = 1; x < image.width() - 1; ++x) {
            QColor center(image.pixel(x, y));
            QColor right(image.pixel(x + 1, y));
            QColor bottom(image.pixel(x, y + 1));

            // Calculate gradients in X and Y directions
            double gradientX = qAbs(center.valueF() - right.valueF());
            double gradientY = qAbs(center.valueF() - bottom.valueF());

            // Total gradient magnitude
            totalGradient += qSqrt(gradientX * gradientX + gradientY * gradientY);
            ++pixelCount;
        }
    }

    return (pixelCount > 0) ? (totalGradient / pixelCount) : 0.0;
}

// Format the comparison result as an HTML string
QString SharpnessComparator::formatResultToHtml(const SharpnessComparisonResult& result) {
    QString html;
    html += "<html><head><style>";
    html += "table { border-collapse: collapse; width: 100%; }";
    html += "th, td { border: 1px solid black; padding: 8px; text-align: left; }";
    html += "th { background-color: #f2f2f2; }";
    html += "</style></head><body>";
    html += "<h3>Image Sharpness Comparison Results</h3><br>";
    html += "<table>";
    html += "<tr><th>Parameter</th><th>Image 1</th><th>Image 2</th></tr>";
    html += QString("<tr><td>Filename</td><td>%1</td><td>%2</td></tr>").arg(result.name1, result.name2);
    html += QString("<tr><td>Sharpness</td><td>%1</td><td>%2</td></tr>")
                .arg(result.sharpness1, 0, 'f', 4)
                .arg(result.sharpness2, 0, 'f', 4);
    html += QString("<tr><td>Sharper Image</td><td colspan='2'>%1</td></tr>")
                .arg(result.sharperImage);
    html += "</table>";
    html += "</body></html>";

    return html;
}

QString SharpnessComparator::name() {
    return "Compare images' sharpness";
}

QString SharpnessComparator::hotkey() {
    return "H";
}

QString SharpnessComparator::description() {
    return QString("This program loads images, calculates their sharpness based on ")
    + "gradient magnitudes of pixel intensity values, compares the results, "
        + "and formats the comparison as an HTML table.";
}

std::shared_ptr<ComparisonResultVariant> SharpnessComparator::compare(ComparableImage first, ComparableImage second) {
    auto result = compareImages(first.getImage(),
                                first.getName(),
                                second.getImage(),
                                second.getName());

    QString html = SharpnessComparator::formatResultToHtml(result);
    return std::make_shared<ComparisonResultVariant>(html);
}
