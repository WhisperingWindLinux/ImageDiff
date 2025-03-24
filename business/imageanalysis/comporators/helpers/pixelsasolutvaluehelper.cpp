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
        QColor(255, 0, 0),    // Red
        QColor(0, 255, 0),    // Green
        QColor(0, 0, 255),    // Blue
        QColor(255, 255, 0),  // Yellow
        QColor(255, 0, 255),  // Magenta
        QColor(0, 255, 255),  // Cyan
        QColor(128, 0, 128),  // Purple
        QColor(128, 128, 0),  // Olive
        QColor(0, 128, 128),  // Teal
        QColor(255, 165, 0),  // Orange
        QColor(75, 0, 130),   // Indigo
        QColor(139, 69, 19),  // Brown
        QColor(220, 20, 60),  // Crimson
        QColor(0, 100, 0),    // Dark Green
        QColor(0, 0, 128),    // Navy
        QColor(128, 128, 128) // Gray
    };

    int colorIndex = 0;
    for (int i = 0; i < ranges.size(); ++i) {
        colorMap[i] = colors[colorIndex];
        colorIndex = (colorIndex + 1) % colors.size();
    }
    return colorMap;
}

QString PixelsAbsolutValueHelper::getColorRangeDescription(bool asHtml) {
    QString description;
    if (asHtml) {
        description =
            "<html>"
            "1. <span style='color: rgb(255, 0, 0);'>Red</span> (255, 0, 0) - Corresponds to Difference Range: 0-0<br>"
            "2. <span style='color: rgb(0, 255, 0);'>Green</span> (0, 255, 0) - Corresponds to Difference Range: 1-1<br>"
            "3. <span style='color: rgb(0, 0, 255);'>Blue</span> (0, 0, 255) - Corresponds to Difference Range: 2-2<br>"
            "4. <span style='color: rgb(255, 255, 0);'>Yellow</span> (255, 255, 0) - Corresponds to Difference Range: 3-3<br>"
            "5. <span style='color: rgb(255, 0, 255);'>Magenta</span> (255, 0, 255) - Corresponds to Difference Range: 4-4<br>"
            "6. <span style='color: rgb(0, 255, 255);'>Cyan</span> (0, 255, 255) - Corresponds to Difference Range: 5-5<br>"
            "7. <span style='color: rgb(128, 0, 128);'>Purple</span> (128, 0, 128) - Corresponds to Difference Range: 6-10<br>"
            "8. <span style='color: rgb(128, 128, 0);'>Olive</span> (128, 128, 0) - Corresponds to Difference Range: 11-15<br>"
            "9. <span style='color: rgb(0, 128, 128);'>Teal</span> (0, 128, 128) - Corresponds to Difference Range: 16-20<br>"
            "10. <span style='color: rgb(255, 165, 0);'>Orange</span> (255, 165, 0) - Corresponds to Difference Range: 21-30<br>"
            "11. <span style='color: rgb(75, 0, 130);'>Indigo</span> (75, 0, 130) - Corresponds to Difference Range: 31-40<br>"
            "12. <span style='color: rgb(139, 69, 19);'>Brown</span> (139, 69, 19) - Corresponds to Difference Range: 41-50<br>"
            "13. <span style='color: rgb(220, 20, 60);'>Crimson</span> (220, 20, 60) - Corresponds to Difference Range: 51-60<br>"
            "14. <span style='color: rgb(0, 100, 0);'>Dark Green</span> (0, 100, 0) - Corresponds to Difference Range: 61-70<br>"
            "15. <span style='color: rgb(0, 0, 128);'>Navy Blue</span> (0, 0, 128) - Corresponds to Difference Range: 71-80<br>"
            "16. <span style='color: rgb(128, 128, 128);'>Gray</span> (128, 128, 128) - Corresponds to Difference Range: >80"
            "</html>";
    } else {
        description =
            "1. Red (255, 0, 0) - Corresponds to Difference Range: 0-0\n"
            "2. Green (0, 255, 0) - Corresponds to Difference Range: 1-1\n"
            "3. Blue (0, 0, 255) - Corresponds to Difference Range: 2-2\n"
            "4. Yellow (255, 255, 0) - Corresponds to Difference Range: 3-3\n"
            "5. Magenta (255, 0, 255) - Corresponds to Difference Range: 4-4\n"
            "6. Cyan (0, 255, 255) - Corresponds to Difference Range: 5-5\n"
            "7. Purple (128, 0, 128) - Corresponds to Difference Range: 6-10\n"
            "8. Olive (128, 128, 0) - Corresponds to Difference Range: 11-15\n"
            "9. Teal (0, 128, 128) - Corresponds to Difference Range: 16-20\n"
            "10. Orange (255, 165, 0) - Corresponds to Difference Range: 21-30\n"
            "11. Indigo (75, 0, 130) - Corresponds to Difference Range: 31-40\n"
            "12. Brown (139, 69, 19) - Corresponds to Difference Range: 41-50\n"
            "13. Crimson (220, 20, 60) - Corresponds to Difference Range: 51-60\n"
            "14. Dark Green (0, 100, 0) - Corresponds to Difference Range: 61-70\n"
            "15. Navy Blue (0, 0, 128) - Corresponds to Difference Range: 71-80\n"
            "16. Gray (128, 128, 128) - Corresponds to Difference Range: >80";
    }
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
