#include "pixelsabsolutevaluecomparator.h"

#include <QCoreApplication>
#include <QImage>
#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QMap>
#include <QDebug>
#include <QLocale>

// Function to calculate pixel differences between two images
QMap<QPair<int, int>, QPair<int, double>> PixelsAbsoluteValueComparator::compareImages(const QImage &image1,
                                                                                       const QImage &image2
                                                                                       )
{
    // General image parameters
    int width = image1.width();
    int height = image1.height();
    int totalPixels = width * height;

    // Define difference ranges
    QList<QPair<int, int>> ranges = {
        {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5},
        {6, 10}, {11, 15}, {16, 20}, {21, 30}, {31, 40},
        {41, 50}, {51, 60}, {61, 70}, {71, 80}, {81, 90},
        {91, 100}, {101, 150}, {151, 200}, {201, 255}
    };

    // Result map to store the count and percentage of differences for each range
    QMap<QPair<int, int>, QPair<int, double>> result;

    // Initialize the count of pixels for each range
    for (const auto &range : ranges) {
        result[range] = qMakePair(0, 0.0);
    }

    // Loop through each pixel and calculate the differences
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color1 = image1.pixelColor(x, y);
            QColor color2 = image2.pixelColor(x, y);

            int diffR = std::abs(color1.red() - color2.red());
            int diffG = std::abs(color1.green() - color2.green());
            int diffB = std::abs(color1.blue() - color2.blue());

            // Find the maximum difference across RGB components
            int maxDiff = std::max({diffR, diffG, diffB});

            // Increment the count for the corresponding range
            for (const auto &range : ranges) {
                if (maxDiff >= range.first && maxDiff <= range.second) {
                    result[range].first++; // Increment pixel count
                    break;
                }
            }
        }
    }

    // Calculate the percentage of pixels for each range
    for (auto it = result.begin(); it != result.end(); ++it) {
        it.value().second = (static_cast<double>(it.value().first) / totalPixels) * 100.0;
    }

    return result;
}

QString PixelsAbsoluteValueComparator::getShortName() const {
    return "Pixels Difference Mapping";
}

QString PixelsAbsoluteValueComparator::getFullName() const {
    return "The difference in color for each pixel between two images";
}

QString PixelsAbsoluteValueComparator::getHotkey() const {
    return "V";
}

QString PixelsAbsoluteValueComparator::getDescription() const {
    return QString("This algorithm compares two images by analyzing ") +
                   "the difference in the colors of each pixel. For each pixel, the " +
                   "difference in color brightness (red, green, blue) is calculated, " +
                   "and based on these differences, the pixels are categorized into " +
                   "predefined ranges. As a result, you can see what percentage of " +
                   "pixels falls into each range of difference, allowing you to " +
                   "assess how similar or different the images are.";
}

ComparisonResultVariantPtr PixelsAbsoluteValueComparator::compare(const ComparableImage &first,
                                                                  const ComparableImage &second
                                                                 )
{
    auto result = compareImages(first.getImage(), second.getImage());

    QString html = PixelsAbsoluteValueComparator::formatResultToHtml(result);
    return std::make_shared<ComparisonResultVariant>(html);
}

QString PixelsAbsoluteValueComparator::formatResultToHtml(const QMap<QPair<int, int>,
                                                                     QPair<int, double>> &result
                                                          )
{
    QLocale locale = QLocale::system();

    QString html;

    html += QString("<h2 style=\"line-height: 2;\">%1</h2>").arg(getFullName());

    html += R"(
        <table border="1" style="
            border-collapse: collapse;
            width: 100%;
            text-align: center;
            table-layout: fixed;">
        <thead>
        <tr style="
            background-color: #f2f2f2;
            font-weight: bold;">
        <th style="padding-left: 10px; padding-right: 10px;">Range</th>
        <th style="padding-left: 10px; padding-right: 10px;">Pixel Count</th>
        <th style="padding-left: 10px; padding-right: 10px;">Pixel Count (%)</th>
        </tr>
        </thead>
        <tbody>
    )";

    for (auto it = result.begin(); it != result.end(); ++it) {
        QString range = QString("%1...%2")
        .arg(it.key().first)
            .arg(it.key().second);
        QString pixelCount = locale.toString(it.value().first);
        QString pixelPercentage = QString::number(it.value().second, 'f', 2) + "%";

        html += "<tr>";
        html += QString("<td align=\"right\" style=\"padding-left: 10px; padding-right: 10px;\">%1</td>").arg(range);
        html += QString("<td align=\"right\" style=\"padding-left: 10px; padding-right: 10px;\">%1</td>").arg(pixelCount);
        html += QString("<td align=\"right\" style=\"padding-left: 10px; padding-right: 10px;\">%1</td>").arg(pixelPercentage);
        html += "</tr>";
    }

    html += "</tbody>";
    html += "</table>";

    html += "<br /><br />";
    html += QString("Range contains the absolute values by which the pixels differ ") +
            "in any of the R, G, or B components.";

    return html;
}
