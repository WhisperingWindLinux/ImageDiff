#include "pixelsasolutvaluehelper.h"

#include <domain/valueobjects/pixeldiffrencerange.h>

PixelsAbsolutValueHelper::PixelsAbsolutValueHelper(PixelsAbsoluteValueComparator::Mode mode)
    : currentMode(mode)
{
}


QList<PixelDifferenceRange> PixelsAbsolutValueHelper::generateDifferenceStringResult(const QImage &image1,
                                                                                     const QImage &image2
                                                                                    )
{
    // General image parameters
    int width = image1.width();
    int height = image1.height();
    int totalPixels = width * height;

    // Define difference ranges
    QList<PixelDifferenceRange> ranges = {
        PixelDifferenceRange(0, 0), PixelDifferenceRange(1, 1), PixelDifferenceRange(2, 2),
        PixelDifferenceRange(3, 3), PixelDifferenceRange(4, 4), PixelDifferenceRange(5, 5),
        PixelDifferenceRange(6, 10), PixelDifferenceRange(11, 15), PixelDifferenceRange(16, 20),
        PixelDifferenceRange(21, 30), PixelDifferenceRange(31, 40), PixelDifferenceRange(41, 50),
        PixelDifferenceRange(51, 60), PixelDifferenceRange(61, 70), PixelDifferenceRange(71, 80),
        PixelDifferenceRange(81, 90), PixelDifferenceRange(91, 100), PixelDifferenceRange(101, 150),
        PixelDifferenceRange(151, 200), PixelDifferenceRange(201, 255)
    };

    // Loop through each pixel and calculate the differences
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color1 = image1.pixelColor(x, y);
            QColor color2 = image2.pixelColor(x, y);
            int maxDiff = calculateDiff(color1, color2);
            // Increment the count for the corresponding range
            for (auto &range : ranges) {
                if (maxDiff >= range.minDifference && maxDiff <= range.maxDifference) {
                    range.pixelCount++; // Increment pixel count
                    break;
                }
            }
        }
    }

    // Calculate the percentage of pixels for each range
    for (auto &range : ranges) {
        range.percentage = (static_cast<double>(range.pixelCount) / totalPixels) * 100.0;
    }

    return ranges;
}

int PixelsAbsolutValueHelper::calculateDiff(QColor color1, QColor color2)
{
    if (currentMode == PixelsAbsoluteValueComparator::Mode::DifferenceBySingleLargestComponent) {
        int diffR = std::abs(color1.red() - color2.red());
        int diffG = std::abs(color1.green() - color2.green());
        int diffB = std::abs(color1.blue() - color2.blue());
        return std::max({diffR, diffG, diffB});
    } else if (currentMode == PixelsAbsoluteValueComparator::Mode::DifferenceByAllComponents) {
        int diff1 = color1.red() + color1.green() + color1.blue();
        int diff2 = color2.red() + color2.green() + color2.blue();
        return std::abs(diff1 - diff2);
    }

    throw std::runtime_error("Bad PixelsAbsoluteValueComparator::Mode "
                             "in PixelsAbsolutValueHelper::calculateDiff.");
}

// Function to generate a color map for each range
std::map<int, QColor> PixelsAbsolutValueHelper::generateColorMap(const QList<PixelDifferenceRange>& ranges) {
    std::map<int, QColor> colorMap;
    QList<QColor> colors = {
        QColor(255, 255, 255), // White
        QColor(255, 0, 0),     // Red
        QColor(0, 255, 0),     // Green
        QColor(0, 0, 255),     // Blue
        QColor(255, 255, 0),   // Yellow
        QColor(255, 0, 255),   // Magenta
        QColor(0, 255, 255),   // Cyan
        QColor(128, 0, 128),   // Purple
        QColor(255, 165, 0),   // Orange
        QColor(139, 69, 19),   // Brown
        QColor(0, 100, 0),     // Dark Green
        QColor(128, 128, 128), // Gray
        QColor(0, 0, 0)        // Black
    };

    int colorIndex = 0;
    for (int i = 0; i < ranges.size(); ++i) {
        colorMap[i] = colors[colorIndex];
        colorIndex = (colorIndex + 1) % colors.size();
    }
    return colorMap;
}

QString PixelsAbsolutValueHelper::getColorRangeDescription() {
    QString description =
        "<html>"
        "<style>"
        //"  .color-box {"
        //"    font-weight: bold;"
        //"  }"
        "  .example {"
        "    font-style: italic;"
        "  }"
        "</style>"
        "<body>"
        "1. <span class='color-box' style='color: black;'>White</span> - Range: 0-0. Example: <span class='example' style='color: rgb(255, 255, 255); background-color: black;'>RGB(255, 255, 255)</span><br>"
        "2. <span class='color-box' style='color: black;'>Red</span> - Range: 1-1. Example: <span class='example' style='color: rgb(255, 0, 0);'>RGB(255, 0, 0).</span><br/>"
        "3. <span class='color-box' style='color: black;'>Green</span> - Range: 2-2. Example: <span class='example' style='color: rgb(0, 255, 0);'>RGB(0, 255, 0).</span><br/>"
        "4. <span class='color-box' style='color: black;'>Blue</span> - Range: 3-3. Example: <span class='example' style='color: rgb(0, 0, 255);'>RGB(0, 0, 255).</span><br/>"
        "5. <span class='color-box' style='color: black;'>Yellow</span> - Range: 4-4. Example: <span class='example' style='color: rgb(255, 255, 0);'>RGB(255, 255, 0).</span><br/>"
        "6. <span class='color-box' style='color: black;'>Magenta</span> - Range: 5-5. Example: <span class='example' style='color: rgb(255, 0, 255);'>RGB(255, 0, 255).</span><br/>"
        "7. <span class='color-box' style='color: black;'>Cyan</span> - Range: 6-10. Example: <span class='example' style='color: rgb(0, 255, 255);'>RGB(0, 255, 255).</span><br/>"
        "8. <span class='color-box' style='color: black;'>Purple</span> - Range: 11-15. Example: <span class='example' style='color: rgb(128, 0, 128);'>RGB(128, 0, 128).</span><br/>"
        "9. <span class='color-box' style='color: black;'>Orange</span> - Range: 16-20. Example: <span class='example' style='color: rgb(255, 165, 0);'>RGB(255, 165, 0).</span><br/>"
        "10. <span class='color-box' style='color: black;'>Brown</span> - Range: 21-30. Example: <span class='example' style='color: rgb(139, 69, 19);'>RGB(139, 69, 19).</span><br/>"
        "11. <span class='color-box' style='color: black;'>Dark Green</span> - Range: 31-40. Example: <span class='example' style='color: rgb(0, 100, 0);'>RGB(0, 100, 0).</span><br/>"
        "12. <span class='color-box' style='color: black;'>Gray</span> - Range: 41-50. Example: <span class='example' style='color: rgb(128, 128, 128);'>RGB(128, 128, 128).</span><br/>"
        "13. <span class='color-box' style='color: black;'>Black</span> - Range: 51-255. Example: <span class='example' style='color: rgb(0, 0, 0);'>RGB(0, 0, 0).</span>"
        "</body>"
        "</html>";
    return description;
}

// Function to generate the difference visualization image
QImage PixelsAbsolutValueHelper::generateDifferenceImage(const QImage &image1,
                                                         const QImage &image2
                                                         )
{
    // Ensure both images have the same dimensions
    if (image1.size() != image2.size()) {
        throw std::runtime_error("Images must have the same dimensions.");
    }

    int width = image1.width();
    int height = image1.height();

    // Define difference ranges
    QList<PixelDifferenceRange> ranges = {
        PixelDifferenceRange(0, 0), PixelDifferenceRange(1, 1), PixelDifferenceRange(2, 2),
        PixelDifferenceRange(3, 3), PixelDifferenceRange(4, 4), PixelDifferenceRange(5, 5),
        PixelDifferenceRange(6, 10), PixelDifferenceRange(11, 15), PixelDifferenceRange(16, 20),
        PixelDifferenceRange(21, 30), PixelDifferenceRange(31, 40), PixelDifferenceRange(41, 50),
        PixelDifferenceRange(51, 60), PixelDifferenceRange(61, 70), PixelDifferenceRange(71, 80),
        PixelDifferenceRange(81, 90), PixelDifferenceRange(91, 100), PixelDifferenceRange(101, 150),
        PixelDifferenceRange(151, 200), PixelDifferenceRange(201, 255)
    };

    // Generate a color map for each range
    std::map<int, QColor> colorMap = generateColorMap(ranges);

    // Create the output image
    QImage outputImage(width, height, QImage::Format_ARGB32);
    outputImage.fill(Qt::white); // Start with a white background

    // Loop through each pixel and calculate the differences
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color1 = image1.pixelColor(x, y);
            QColor color2 = image2.pixelColor(x, y);
            int maxDiff = calculateDiff(color1, color2);

            // Find the corresponding range for the difference and apply the color
            for (int i = 0; i < ranges.size(); ++i) {
                const auto& range = ranges[i];
                if (maxDiff >= range.minDifference && maxDiff <= range.maxDifference) {
                    // White background mode: draw only differing pixels
                    outputImage.setPixelColor(x, y, colorMap[i]);
                    break;
                }
            }
        }
    }

    return outputImage;
}
