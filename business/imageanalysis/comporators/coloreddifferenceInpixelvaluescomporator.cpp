#include "coloreddifferenceInpixelvaluescomporator.h"

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
#include <business/imageanalysis/comporators/helpers/pixelsasolutvaluehelper.h>


ColoredDifferenceInPixelValuesComporator::ColoredDifferenceInPixelValuesComporator(Result result) {
    currentMode = PixelsDifferenceCalculationMode::DifferenceBySingleLargestComponent;
    expectedResult = result;
}

void ColoredDifferenceInPixelValuesComporator::setProperties(QList<Property> properties) {
    if (properties.size() != 1) {
        QString error = "Got an error from %1: an incorrect number of properties.";
        error = error.arg(getShortName());
        throw std::runtime_error(error.toStdString());
    }
    int prop1Index = properties[0].getValue();

    currentMode = (prop1Index == 0 ?
                       PixelsDifferenceCalculationMode::DifferenceBySingleLargestComponent :
                       PixelsDifferenceCalculationMode::DifferenceByAllComponents
                   );
}

QList<Property> ColoredDifferenceInPixelValuesComporator::getDefaultProperties() const {
    QString prop1Description = "There are two modes of operation for the algorithm: "
                               "DifferenceBySingleLargestComponent and DifferenceByAllComponents. "
                               "In the first case, the algorithm calculates the absolute difference "
                               "for each component (R, G, or B) and takes the largest value among "
                               "them, which is then used as the comparison value.  In the second c"
                               "ase, the absolute differences of all components (R, G, and B) are "
                               "summed up, and their total is used as the comparison value.";

    QList<QString> prop1Alternatives = { "DifferenceBySingleLargestComponent", "DifferenceByAllComponents" };
    auto prop1 = Property::createAlternativesProperty("Mode", prop1Description, prop1Alternatives, 0);
    return { prop1 };
}

void ColoredDifferenceInPixelValuesComporator::reset() {
    currentMode = PixelsDifferenceCalculationMode::DifferenceBySingleLargestComponent;
}

QString ColoredDifferenceInPixelValuesComporator::getShortName() const {
    if (expectedResult == Result::Text) {
        return "Difference In Pixel Values v.2 (Text)";
    } else {
        return "Difference In Pixel Values v.2 (Image, Multicolor)";
    }
}

QString ColoredDifferenceInPixelValuesComporator::getFullName() const {
    if (expectedResult == Result::Text) {
        return "Difference In Pixel Values v.2 (Text)";
    } else {
        return "Difference In Pixel Values v.2 (Image, Multicolor)";
    }
}

QString ColoredDifferenceInPixelValuesComporator::getHotkey() const {
    return (expectedResult == Result::Text ? "v" : "m");
}

QString ColoredDifferenceInPixelValuesComporator::getDescription() const {
    QString baseHelpTxt = QString("This algorithm compares two images by analyzing ") +
                          "the difference in the colors of each pixel. For each pixel, the " +
                          "difference in color brightness (red, green, blue) is calculated, " +
                          "and based on these differences, the pixels are categorized into " +
                          "predefined ranges.";

    if (expectedResult == Result::Text) {
        return baseHelpTxt + " It shows the result in text form.";
    } else {
        QString colorRangeDexcription = PixelsAbsolutValueHelper::getColorRangeDescription();
        baseHelpTxt += " It shows the result as an image. As pecific color is used to indicate "
                       "the pixel's belonging to a particular range.<br/>";
        baseHelpTxt += colorRangeDexcription;
        return baseHelpTxt;
    }
}

ComparisonResultVariantPtr ColoredDifferenceInPixelValuesComporator::compare(const ComparableImage &first,
                                                                  const ComparableImage &second
                                                                  )
{
    PixelsAbsolutValueHelper helper { currentMode };

    if (expectedResult == Result::Text) {
        QList<PixelDifferenceRange> ranges = helper.generateDifferenceStringResult(first.getImage(),
                                                                                   second.getImage()
                                                                                   );

        QString result = PixelsAbsolutValueFormatter::formatResultToHtml(ranges,
                                                                         getFullName(),
                                                                         currentMode
                                                                         );
        return std::make_shared<ComparisonResultVariant>(result);

    } else if (expectedResult == Result::Image){
        QImage result = helper.generateDifferenceImage(first.getImage(), second.getImage());
        return std::make_shared<ComparisonResultVariant>(result);
    }

    throw runtime_error("Bad Result type in PixelsAbsoluteValueComparator::compare.");
}
