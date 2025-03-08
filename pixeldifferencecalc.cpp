#include <QCoreApplication>
#include <QImage>
#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QMap>
#include <QPair>
#include <QDebug>

#include "pixeldifferencecalc.h"

// Function to calculate pixel differences between two images
QMap<QPair<int, int>, QPair<int, double> > PixelDifferenceCalc::calculatePixelDifferences(const QString &image1Path, const QString &image2Path) {
    // Load the images
    QImage img1(image1Path);
    QImage img2(image2Path);

    // Check if images were successfully loaded
    if (img1.isNull() || img2.isNull()) {
        throw std::runtime_error("One or both images could not be loaded!");
    }

    // Check if the sizes of the images match
    if (img1.size() != img2.size()) {
        throw std::runtime_error("The sizes of the images do not match!");
    }

    // General image parameters
    int width = img1.width();
    int height = img1.height();
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
            QColor color1 = img1.pixelColor(x, y);
            QColor color2 = img2.pixelColor(x, y);

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
