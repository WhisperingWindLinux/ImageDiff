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
#include <business/imageanalysis/comporators/helpers/pixelsasolutvaluehelper.h>


CustomRangedDifferenceInPixelValuesComparator::CustomRangedDifferenceInPixelValuesComparator() {
    mStartOfRange = 1;
    mEndOfRange = 255;
}

void CustomRangedDifferenceInPixelValuesComparator::setProperties(QList<Property> properties) {
    if (properties.size() != 2) {
        QString error = "Got an error from %1: an incorrect number of properties.";
        error = error.arg(getShortName());
        throw std::runtime_error(error.toStdString());
    }
    mStartOfRange = properties[0].getValue();
    mEndOfRange = properties[1].getValue();
}

QList<Property> CustomRangedDifferenceInPixelValuesComparator::getDefaultProperties() const {
    auto prop1 = Property::createIntProperty("Start of the range", "Start of the custom range.", mStartOfRange, 0, 255);
    auto prop2 = Property::createIntProperty("End of the range", "End of the custom range.", mEndOfRange, 0, 255);

    return { prop1, prop2, };
}

void CustomRangedDifferenceInPixelValuesComparator::reset() {
    mStartOfRange = 1;
    mEndOfRange = 255;
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
    return "U";
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
    if (mStartOfRange > mEndOfRange) {
        throw std::runtime_error("The start of the range cannot be greater than its end.");
    }

    PixelsAbsolutValueHelper helper {};

    QImage result = helper.generateDifferenceImageByCustomRage(first.getImage(),
                                                               second.getImage(),
                                                               mStartOfRange,
                                                               mEndOfRange
                                                               );
    return std::make_shared<ComparisonResultVariant>(result);
}
