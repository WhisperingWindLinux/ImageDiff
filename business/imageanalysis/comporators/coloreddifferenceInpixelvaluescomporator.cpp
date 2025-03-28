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
    expectedResult = result;
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
    PixelsAbsolutValueHelper helper {};

    if (expectedResult == Result::Text) {
        QList<PixelDifferenceRange> ranges = helper.generateDifferenceStringResult(first.getImage(),
                                                                                   second.getImage()
                                                                                  );

        QString result = PixelsAbsolutValueFormatter::formatResultToHtml(ranges,
                                                                         getFullName()
                                                                        );
        return std::make_shared<ComparisonResultVariant>(result);

    } else if (expectedResult == Result::Image){
        QImage result = helper.generateDifferenceImage(first.getImage(), second.getImage());
        return std::make_shared<ComparisonResultVariant>(result);
    }

    throw runtime_error("Bad Result type in PixelsAbsoluteValueComparator::compare.");
}
