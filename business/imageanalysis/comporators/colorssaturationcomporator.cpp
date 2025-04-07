#include "colorssaturationcomporator.h"

#include <QFileInfo>
#include <QDebug>

#include <business/imageanalysis/comporators/helpers/mathhelper.h>

// Compare the two images and return a structure with the results
ColorsSaturationComparisonResult ColorsSaturationComporator::compareImages(const QImage &image1,
                                                                           const QString &name1,
                                                                           const QImage &image2,
                                                                           const QString &name2
                                                                           )
{

    if (image1.isNull() || image2.isNull()) {
        qWarning() << "Error: Unable to load one or both images.";
        return {};
    }

    double avgSat1 = calculateAverageSaturation(image1);
    double avgSat2 = calculateAverageSaturation(image2);

    return {name1, name2, avgSat1, avgSat2};
}

// Calculate the average saturation of an image
double ColorsSaturationComporator::calculateAverageSaturation(const QImage &image) {
    double totalSaturation = 0.0;
    int pixelCount = 0;

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color(image.pixel(x, y));
            totalSaturation += color.toHsv().saturationF();  // Saturation in range [0.0, 1.0]
            ++pixelCount;
        }
    }

    return (pixelCount > 0) ? (totalSaturation / pixelCount) : 0.0 ;
}

QString ColorsSaturationComporator::getShortName() const {
    return "Saturation";
}

QString ColorsSaturationComporator::getFullName() const {
    return "Comparison of image saturation";
}

QString ColorsSaturationComporator::getHotkey() const {
    return "T";
}

QString ColorsSaturationComporator::getDescription() const {
    return QString("This algorithm calculates average saturation based on pixel HSV values. ")
                   + "The range of claculated coefficient values is [0.0, 1.0]."
                   + " Where higher values indicate a more saturated image.";
}

ComparisonResultVariantPtr ColorsSaturationComporator::compare(const ComparableImage& first,
                                                               const ComparableImage& second
                                                              )
{
    auto result = compareImages(first.getImage(),
                                first.getImageName(),
                                second.getImage(),
                                second.getImageName()
                                );

    QString html = ColorsSaturationComporator::formatResultToHtml(result);
    return std::make_shared<ComparisonResultVariant>(html);
}

QString ColorsSaturationComporator::formatResultToHtml(const ColorsSaturationComparisonResult& result) {
    QString html;

    auto raundedResult = MathHelper::roundAndCompare(result.firstImageSaturation, result.secondImageSaturation);

    auto beautifyPercentage = MathHelper::calcAndBeautifyPercentageValue(raundedResult.value1,
                                                                         raundedResult.value2,
                                                                         result.firstImageName,
                                                                         result.secondImageName,
                                                                         "Equally"
                                                                         );

    html += QString("<h2 style=\"line-height: 2;\">%1</h2>").arg(getFullName());
    html += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">";
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(result.firstImageName, raundedResult.string1);
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(result.secondImageName, raundedResult.string2);
    if (beautifyPercentage.isEqually) {
        html += QString("<tr><td colspan=\"2\" align=\"center\">%1</td></tr>")
                    .arg(beautifyPercentage.resultDescription);
    } else {
        html += QString("<tr><td colspan=\"2\" align=\"center\">Image <b>"
                        "<font color=\"green\">%1</font></b> is %2 more saturated</td></tr>")
                    .arg(beautifyPercentage.resultDescription, beautifyPercentage.percentageResult);
    }

    html += "</table>";
    html += "<br /><br />";
    html += QString("The range of coefficient values is [0.0, 1.0]. ")
            + "Higher values indicate a more saturated image.";

    return html;
}
