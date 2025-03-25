#include "customrangeddifferenceinpixelvaluescomparator.h"

#include <QCoreApplication>
#include <QImage>
#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QMap>
#include <QDebug>
#include <QLocale>
#include <domain/valueobjects/pixeldiffrencerange.h>
#include <business/imageanalysis/comporators/formatters/pixelsabsolutevalueformatter.h>


CustomRangedDifferenceInPixelValuesComparator::CustomRangedDifferenceInPixelValuesComparator() {
    currentMode = PixelsDifferenceCalculationMode::DifferenceBySingleLargestComponent;
    startOfRange = 1;
    endOfRange = 255;
}

void CustomRangedDifferenceInPixelValuesComparator::setProperties(QList<Property> properties) {
    if (properties.size() != 3) {
        QString error = "Got an error from %1: an incorrect number of properties.";
        error = error.arg(getShortName());
        throw std::runtime_error(error.toStdString());
    }
    int prop1Index = properties[0].getValue();

    currentMode = (prop1Index == 0 ?
                       PixelsDifferenceCalculationMode::DifferenceBySingleLargestComponent :
                       PixelsDifferenceCalculationMode::DifferenceByAllComponents
                   );

    startOfRange = properties[1].getValue();
    endOfRange = properties[2].getValue();

}

QList<Property> CustomRangedDifferenceInPixelValuesComparator::getDefaultProperties() const {
    QString prop1Description = "There are two modes of operation for the algorithm: "
                               "DifferenceBySingleLargestComponent and DifferenceByAllComponents. "
                               "In the first case, the algorithm calculates the absolute difference "
                               "for each component (R, G, or B) and takes the largest value among "
                               "them, which is then used as the comparison value.  In the second c"
                               "ase, the absolute differences of all components (R, G, and B) are "
                               "summed up, and their total is used as the comparison value.";

    QList<QString> prop1Alternatives = { "DifferenceBySingleLargestComponent", "DifferenceByAllComponents" };
    auto prop1 = Property::createAlternativesProperty("Mode", prop1Description, prop1Alternatives, 0);
    auto prop2 = Property::createIntProperty("Start of the range", "Start of the custom range.", startOfRange, 0, 255);
    auto prop3 = Property::createIntProperty("End of the range", "End of the custom range.", endOfRange, 0, 255);

    return { prop1, prop2, prop3 };
}

void CustomRangedDifferenceInPixelValuesComparator::reset() {
    currentMode = PixelsDifferenceCalculationMode::DifferenceBySingleLargestComponent;
    startOfRange = 1;
    endOfRange = 255;
}

bool CustomRangedDifferenceInPixelValuesComparator::isPartOfAutoReportingToolbox() {
    return false;
}

QString CustomRangedDifferenceInPixelValuesComparator::getShortName() const {
    return "Difference In Pixel Values v.4 (Image, Custom Range, Single Color)";
}

QString CustomRangedDifferenceInPixelValuesComparator::getFullName() const {
    return "Difference In Pixel Values v.4 (Image, Custom Range, Single Color)";
}

QString CustomRangedDifferenceInPixelValuesComparator::getHotkey() const {
    return "u";
}

QString CustomRangedDifferenceInPixelValuesComparator::getDescription() const {
    QString helpTxt = QString("This algorithm compares two images by analyzing ") +
                      "the difference in the colors of each pixel. For each pixel, the " +
                      "difference in color brightness (red, green, blue) is calculated, " +
                      "and based on these differences, the pixels are categorized into " +
                      "predefined ranges. It displays the result as an image, where red dots " +
                      "represent pixels that differ between the two images within the selected range.";

    return helpTxt;
}

ComparisonResultVariantPtr CustomRangedDifferenceInPixelValuesComparator::compare(
                                                                        const ComparableImage &first,
                                                                        const ComparableImage &second
                                                                      )
{
    if (startOfRange > endOfRange) {
        throw std::runtime_error("The start of the range cannot be greater than its end.");
    }

    PixelsAbsolutValueHelper helper { currentMode };

    QImage result = helper.generateDifferenceImageByCustomRage(first.getImage(),
                                                               second.getImage(),
                                                               startOfRange,
                                                               endOfRange
                                                               );
    return std::make_shared<ComparisonResultVariant>(result);
}
