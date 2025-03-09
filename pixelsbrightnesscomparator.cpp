#include <QImage>
#include <QString>
#include <QColor>
#include <QDebug>
#include <qfileinfo.h>

#include "pixelsbrightnesscomparator.h"

ComparisonResult PixelsBrightnessComparator::compareImages(const QString& imagePath1, const QString& imagePath2) {
    QImage image1(imagePath1);
    QImage image2(imagePath2);

    // Check if images are loaded successfully
    if (image1.isNull() || image2.isNull()) {
        throw std::runtime_error("Error: One or both images could not be loaded.");
    }

    // Ensure images have the same dimensions
    if (image1.size() != image2.size()) {
        throw std::runtime_error("Error: Images must have the same dimensions.");
    }

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

    result.firstImagePath = imagePath1;
    result.secondImagePath = imagePath2;

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
    QFileInfo file1(result.firstImagePath);
    QFileInfo file2(result.secondImagePath);

    QString file1Name, file2Name;

    if (file1.fileName() == file2.fileName()) {
        file1Name = file1.absoluteFilePath();
        file2Name = file2.absoluteFilePath();
    } else {
        file1Name = file1.fileName();
        file2Name = file2.fileName();
    }

    QString html;
    html += QString("<b>Pixel Brightness Difference Analysis for %1</b><br>")
        .arg(file1Name);
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
        .arg(file1Name)
            .arg(file2Name)
            .arg(result.totalBrightness1)
            .arg(result.totalBrightness2)
            .arg(QString::number(brightnessDifferencePercentage, 'f', 2));
    } else if (dTotalBrightness1 < dTotalBrightness2) {
        html += QString("<br><br>%1 is darker than %2 because its total brightness (%3) is less than the second's (%4) by %5%.")
        .arg(file1Name)
            .arg(file2Name)
            .arg(result.totalBrightness1)
            .arg(result.totalBrightness2)
            .arg(QString::number(brightnessDifferencePercentage, 'f', 2));
    } else {
        html += "<br><br>Both images have the same total brightness.";
    }

    return html;
}

