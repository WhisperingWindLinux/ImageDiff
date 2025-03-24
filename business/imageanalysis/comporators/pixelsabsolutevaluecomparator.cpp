#include "pixelsabsolutevaluecomparator.h"

#include <QCoreApplication>
#include <QImage>
#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QMap>
#include <QDebug>
#include <QLocale>
#include <domain/valueobjects/pixeldiffrencerange.h>
#include <business/imageanalysis/comporators/formatters/pixelsabsolutvaluescomparatorformatter.h>


PixelsAbsoluteValueComparator::PixelsAbsoluteValueComparator() {
    currentMode = Mode::DifferenceBySingleLargestComponent;
}

// Function to calculate pixel differences between two images
QList<PixelDifferenceRange> PixelsAbsoluteValueComparator::compareImages(const QImage &image1,
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

int PixelsAbsoluteValueComparator::calculateDiff(QColor color1, QColor color2)
{
    if (currentMode == Mode::DifferenceBySingleLargestComponent) {
        int diffR = std::abs(color1.red() - color2.red());
        int diffG = std::abs(color1.green() - color2.green());
        int diffB = std::abs(color1.blue() - color2.blue());
        return std::max({diffR, diffG, diffB});
    } else if (currentMode == Mode::DifferenceByAllComponents) {
        int diff1 = color1.red() + color1.green() + color1.blue();
        int diff2 = color2.red() + color2.green() + color2.blue();
        return std::abs(diff1 - diff2);
    } else {
        QString error = "Got an error from '%1': an incorrect mode.";
        error = error.arg(getShortName());
        throw std::runtime_error(error.toStdString());
    }
}

QList<Property> PixelsAbsoluteValueComparator::getDefaultProperties() const {
    QString description = "There are two modes of operation for the algorithm: "
                          "DifferenceBySingleLargestComponent and DifferenceByAllComponents. "
                          "In the first case, the algorithm calculates the absolute difference "
                          "for each component (R, G, or B) and takes the largest value among "
                          "them, which is then used as the comparison value.  In the second c"
                          "ase, the absolute differences of all components (R, G, and B) are "
                          "summed up, and their total is used as the comparison value.";

    QList<QString> alternatives = { "DifferenceBySingleLargestComponent", "DifferenceByAllComponents" };
    auto altsProperty = Property::createAlternativesProperty("Mode", description, alternatives, 0);
    return { altsProperty };
}

void PixelsAbsoluteValueComparator::setProperties(QList<Property> properties) {
    if (properties.size() != 1) {
        QString error = "Got an error from %1: an incorrect number of properties.";
        error = error.arg(getShortName());
        throw std::runtime_error(error.toStdString());
    }
    int altsIndex = properties[0].getValue();

    currentMode = (altsIndex == 0 ?
                       Mode::DifferenceBySingleLargestComponent :
                       Mode::DifferenceByAllComponents
                   );
}

void PixelsAbsoluteValueComparator::reset() {
    currentMode = Mode::DifferenceBySingleLargestComponent;
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
           "assess how similar or different the images are.<br/>" +
           "There are two modes of operation for the algorithm: " +
           "DifferenceBySingleLargestComponent and DifferenceByAllComponents. " +
           "In the first case, the algorithm calculates the absolute difference " +
           "for each component (R, G, or B) and takes the largest value among " +
           "them, which is then used as the comparison value.  In the second c" +
           "ase, the absolute differences of all components (R, G, and B) are " +
           "summed up, and their total is used as the comparison value.";
}

ComparisonResultVariantPtr PixelsAbsoluteValueComparator::compare(const ComparableImage &first,
                                                                  const ComparableImage &second
                                                                 )
{
    auto result = compareImages(first.getImage(), second.getImage());

    QString html = PixelsAbsolutValuesComparatorFormatter::formatResultToHtml(result, getFullName(), currentMode);
    return std::make_shared<ComparisonResultVariant>(html);
}
