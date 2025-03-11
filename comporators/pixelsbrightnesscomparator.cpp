#include <QImage>
#include <QString>
#include <QColor>
#include <QDebug>
#include <qfileinfo.h>

#include "pixelsbrightnesscomparator.h"


ComparisonResult PixelsBrightnessComparator::compareImages(QImage image1,
                                                           QString name1,
                                                           QImage image2,
                                                           QString name2
                                                           )
{
    int width = image1.width();
    int height = image1.height();
    int totalPixels = width * height;

    int sameColorCount = 0;
    int brighterCount = 0;
    int darkerCount = 0;

    // Variables to store total brightness of each image
    qint64 totalBrightness1 = 0;
    qint64 totalBrightness2 = 0;

    // Iterate through each pixel and compare colors
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color1 = image1.pixelColor(x, y);
            QColor color2 = image2.pixelColor(x, y);

            int brightness1 = qGray(color1.rgb());
            int brightness2 = qGray(color2.rgb());

            totalBrightness1 += brightness1;
            totalBrightness2 += brightness2;

            if (color1 == color2) {
                ++sameColorCount;
            } else if (brightness1 > brightness2) {
                ++brighterCount;
            } else {
                ++darkerCount;
            }
        }
    }

    // Populate the result structure
    ComparisonResult result;

    result.name1 = name1;
    result.name2 = name2;

    result.totalPixels = totalPixels;
    result.sameColorCount = sameColorCount;
    result.sameColorPercent = (static_cast<double>(sameColorCount) / totalPixels) * 100;

    result.brighterCount = brighterCount;
    result.brighterPercent = (static_cast<double>(brighterCount) / totalPixels) * 100;

    result.darkerCount = darkerCount;
    result.darkerPercent = (static_cast<double>(darkerCount) / totalPixels) * 100;

    // Store total brightness values in the result
    result.totalBrightness1 = totalBrightness1;
    result.totalBrightness2 = totalBrightness2;

    return result;
}

// Function to format comparison results as an HTML table
QString PixelsBrightnessComparator::formatResultToHtml(const ComparisonResult& result) {

    QString html;
    html += QString("<b>Pixel Brightness Difference Analysis for %1</b><br>")
        .arg(result.name1);
    html += "<table border='1' cellpadding='5' cellspacing='0'>";
    html += "<tr><th>Metric</th><th>Value</th></tr>";
    html += QString("<tr><td>Total Pixels</td><td>%1</td></tr>").arg(result.totalPixels);
    html += QString("<tr><td>Same Color Count</td><td>%1 (%2%)</td></tr>")
                .arg(result.sameColorCount)
                .arg(result.sameColorPercent, 0, 'f', 2);
    html += QString("<tr><td>Brighter Count</td><td>%1 (%2%)</td></tr>")
                .arg(result.brighterCount)
                .arg(result.brighterPercent, 0, 'f', 2);
    html += QString("<tr><td>Darker Count</td><td>%1 (%2%)</td></tr>")
                .arg(result.darkerCount)
                .arg(result.darkerPercent, 0, 'f', 2);
    html += "</table>";

    double dTotalBrightness1 = result.totalBrightness1;
    double dTotalBrightness2 = result.totalBrightness2;

    // Add a line describing which image is brighter
    double brightnessDifferencePercentage = (std::abs(dTotalBrightness1 - dTotalBrightness2) /
                                             std::max(dTotalBrightness1, dTotalBrightness2)) * 100;

    if (dTotalBrightness1 > dTotalBrightness2) {
        html += QString("<br><br>%1 is brighter than %2 because its total brightness (%3) is greater than the second's (%4) by %5%.")
        .arg(result.name1)
            .arg(result.name2)
            .arg(result.totalBrightness1)
            .arg(result.totalBrightness2)
            .arg(QString::number(brightnessDifferencePercentage, 'f', 2));
    } else if (dTotalBrightness1 < dTotalBrightness2) {
        html += QString("<br><br>%1 is darker than %2 because its total brightness (%3) is less than the second's (%4) by %5%.")
        .arg(result.name1)
            .arg(result.name2)
            .arg(result.totalBrightness1)
            .arg(result.totalBrightness2)
            .arg(QString::number(brightnessDifferencePercentage, 'f', 2));
    } else {
        html += "<br><br>Both images have the same total brightness.";
    }

    return html;
}

QString PixelsBrightnessComparator::name() {
    return "Show pixels' brigthness difference statistics";
}

QString PixelsBrightnessComparator::hotkey() {
    return "N";
}

QString PixelsBrightnessComparator::description() {
    return name() + '.';
}

std::shared_ptr<ComparisonResultVariant> PixelsBrightnessComparator::compare(ComparableImage first,
                                                                             ComparableImage second
                                                                             )
{
    auto result = compareImages(first.getImage(),
                                first.getName(),
                                second.getImage(),
                                second.getName()
                                );

    QString html = PixelsBrightnessComparator::formatResultToHtml(result);
    std::shared_ptr<ComparisonResultVariant> resultVariant = std::make_shared<ComparisonResultVariant>(html);
    return resultVariant;
}
