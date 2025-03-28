#include <QImage>
#include <QString>
#include <QColor>
#include <QDebug>
#include <qfileinfo.h>

#include <business/imageanalysis/comporators/helpers/mathhelper.h>

#include "pixelsbrightnesscomparator.h"


PixelsBrightnessComparisonResult PixelsBrightnessComparator::compareImages(const QImage &image1,
                                                                           const QString& name1,
                                                                           const QImage &image2,
                                                                           const QString& name2
                                                                          )
{
    int width = image1.width();
    int height = image1.height();
    int totalPixels = width * height;

    int sameColorCount = 0;
    int brighterCount = 0;
    int darkerCount = 0;

    // Variables to store total brightness of each image
    qint64 totalBrightness1 = 0;
    qint64 totalBrightness2 = 0;

    // Iterate through each pixel and compare colors
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color1 = image1.pixelColor(x, y);
            QColor color2 = image2.pixelColor(x, y);

            int brightness1 = qGray(color1.rgb());
            int brightness2 = qGray(color2.rgb());

            totalBrightness1 += brightness1;
            totalBrightness2 += brightness2;

            if (color1 == color2) {
                ++sameColorCount;
            } else if (brightness1 > brightness2) {
                ++brighterCount;
            } else {
                ++darkerCount;
            }
        }
    }

    // Populate the result structure
    PixelsBrightnessComparisonResult result;

    result.name1 = name1;
    result.name2 = name2;

    result.totalPixels = totalPixels;
    result.sameColorCount = sameColorCount;
    result.sameColorPercent = (static_cast<double>(sameColorCount) / totalPixels) * 100;

    result.brighterCount = brighterCount;
    result.brighterPercent = (static_cast<double>(brighterCount) / totalPixels) * 100;

    result.darkerCount = darkerCount;
    result.darkerPercent = (static_cast<double>(darkerCount) / totalPixels) * 100;

    // Store total brightness values in the result
    result.totalBrightness1 = totalBrightness1;
    result.totalBrightness2 = totalBrightness2;

    return result;
}

QString PixelsBrightnessComparator::getShortName() const {
    return "Brigthness";
}

QString PixelsBrightnessComparator::getFullName() const {
    return "Comparison of image brightness";
}

QString PixelsBrightnessComparator::getHotkey() const {
    return "N";
}

QString PixelsBrightnessComparator::getDescription() const {
    return QString("This algorithm compares the brightness of corresponding pixels in two images. ")
                   + "The result provides percentages of pixels with the same color, "
                   + "brighter pixels in the first image, and darker pixels in the second image. "
                   + "The algorithm makes a conclusion based on the total "
                   + "brightness of all pixels. This means that there may be fewer brighter "
                   + "pixels, but their total brightness will be higher than that of"
                   + " the second image.";
}

ComparisonResultVariantPtr PixelsBrightnessComparator::compare(const ComparableImage &first,
                                                               const ComparableImage &second
                                                              )
{
    auto result = compareImages(first.getImage(),
                                first.getName(),
                                second.getImage(),
                                second.getName()
                                );

    QString html = PixelsBrightnessComparator::formatResultToHtml(result);
    ComparisonResultVariantPtr resultVariant = std::make_shared<ComparisonResultVariant>(html);
    return resultVariant;
}

QString PixelsBrightnessComparator::formatResultToHtml(const PixelsBrightnessComparisonResult &result) {

    QLocale locale = QLocale::system();
    QString formattedTotalPixels = locale.toString(result.totalPixels);
    QString formattedTheSameCount = locale.toString(result.sameColorCount);
    QString formattedBrighterCount = locale.toString(result.brighterCount);
    QString formattedDarkerCount = locale.toString(result.darkerCount);

    QString formattedTotalBrightness1 = locale.toString(result.totalBrightness1);
    QString formattedTotalBrightness2 = locale.toString(result.totalBrightness2);

    QString formattedSameColorPercent = MathHelper::formatPersentageValue(result.sameColorPercent);
    QString formattedBrighterPercent = MathHelper::formatPersentageValue(result.brighterPercent);
    QString formattedDarkerPercent = MathHelper::formatPersentageValue(result.darkerPercent);


    QString html;

    html += QString("<h2 style=\"line-height: 2;\">%1</h2>").arg(getFullName());
    html += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">";

    html += QString("<tr><td align=\"center\"><b>%1</b></td><td align=\"center\"><b>%2</b></td><td align=\"center\"><b>%3</b></td></tr>")
                .arg("Parameter")
                .arg("Pixel Count")
                .arg("Pixel Count (%)");
    html += QString("<tr><td>Total pixels</td><td align=\"right\">%1</td><td align=\"right\">100%</td></tr>")
                .arg(formattedTotalPixels);
    html += QString("<tr><td>Pixels with the same brightness in %1 and %2</td><td align=\"right\">%3</td><td align=\"right\">%4</td></tr>")
                .arg(result.name1)
                .arg(result.name2)
                .arg(formattedTheSameCount)
                .arg(formattedSameColorPercent);
    html += QString("<tr><td>Brighter pixels in %1 than in %2</td><td align=\"right\">%3</td><td align=\"right\">%4</td></tr>")
                .arg(result.name1)
                .arg(result.name2)
                .arg(formattedBrighterCount)
                .arg(formattedBrighterPercent);
    html += QString("<tr><td>Darker pixels in %1 than in %2</td><td align=\"right\">%3</td><td align=\"right\">%4</td></tr>")
                .arg(result.name1)
                .arg(result.name2)
                .arg(formattedDarkerCount)
                .arg(formattedDarkerPercent);
    html += "</table>";
    html += "<br/>";

    html += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">";
    html += QString("<tr><td align=\"center\"><b>%1</b></td><td align=\"center\"><b>%2</b></td></tr>")
                .arg("Parameter")
                .arg("Value");
    html += QString("<tr><td>Overall brightness %1</td><td align=\"right\">%2</td></tr>")
                .arg(result.name1)
                .arg(formattedTotalBrightness1);
    html += QString("<tr><td>Overall brightness %1</td><td align=\"right\">%2</td></tr>")
                .arg(result.name2)
                .arg(formattedTotalBrightness2);


    double dTotalBrightness1 = result.totalBrightness1;
    double dTotalBrightness2 = result.totalBrightness2;

    auto beautifyPesantage = MathHelper::calcAndBeautifyPersantageValue(dTotalBrightness1,
                                                                        dTotalBrightness2,
                                                                        "",
                                                                        "",
                                                                        "");

    html += "<tr><td colspan=\"2\" align=\"center\">";

    if (dTotalBrightness1 > dTotalBrightness2) {
        html += QString("Image <b><font color=\"green\">%1</font></b> is %2 brighter")
            .arg(result.name1)
            .arg(beautifyPesantage.persantageResult);
    } else if (dTotalBrightness2 > dTotalBrightness1) {
        html += QString("Image <b><font color=\"green\">%1</font></b> is %2 brighter")
            .arg(result.name2)
            .arg(beautifyPesantage.persantageResult);
    } else {
        html += "Equally";
    }

    html += "</tr>";
    html += "</table>";
    html += "<br/><br/>";
    html += QString("The algorithm makes a conclusion based on the total ")
            + "brightness of all pixels. This means that there may be fewer brighter "
            + "pixels, but their total brightness will be higher than that of"
            + " the second image.";

    return html;
}
