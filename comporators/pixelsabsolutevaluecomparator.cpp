#include <QCoreApplication>
#include <QImage>
#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QMap>
#include <QDebug>

#include "pixelsabsolutevaluecomparator.h"

// Function to calculate pixel differences between two images
QMap<QPair<int, int>, QPair<int, double>> PixelsAbsoluteValueComparator::compareImages(QImage image1,
                                                                                       QImage image2
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

QString PixelsAbsoluteValueComparator::formatResultToHtml(const QMap<QPair<int, int>, QPair<int, double>> &result) {

    QString resultText;
    resultText.append("<h3>Pixel Shade Differences by Range</h3><br><br>");

    for (auto it = result.begin(); it != result.end(); ++it) {
        resultText.append(QString("Range <font color=\"green\">[%1-%2]</font>: %3 pixels (<font color=\"red\">%4%</font>)<br>")
                              .arg(it.key().first)
                              .arg(it.key().second)
                              .arg(it.value().first)
                              .arg(QString::number(it.value().second, 'f', 2)));
    }
    return resultText;
}

QString PixelsAbsoluteValueComparator::name() {
    return "Show the absolute difference statistics for pixel values";
}

QString PixelsAbsoluteValueComparator::hotkey() {
    return "V";
}

QString PixelsAbsoluteValueComparator::description() {
    return name() + ".";
}

std::shared_ptr<ComparisonResultVariant> PixelsAbsoluteValueComparator::compare(ComparableImage first,
                                                                                ComparableImage second
                                                                                )
{
    auto result = compareImages(first.getImage(), second.getImage());

    QString html = PixelsAbsoluteValueComparator::formatResultToHtml(result);
    std::shared_ptr<ComparisonResultVariant> resultVariant = std::make_shared<ComparisonResultVariant>(html);
    return resultVariant;
}
