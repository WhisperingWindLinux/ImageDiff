#include <QFileInfo>
#include <cmath>

#include "contrastcomporator.h"

// Method to compare the contrast of two images
ContrastComparisonResult ContrastComporator::compareImages(const QImage &image1,
                                                           const QString &name1,
                                                           const QImage &image2,
                                                           const QString &name2
                                                           )
{
    // Calculate contrast for both images
    double contrast1 = calculateContrast(image1);
    double contrast2 = calculateContrast(image2);

    // Determine which image has higher contrast
    QString moreContrastImage;
    if (contrast1 > contrast2) {
        moreContrastImage = name1;
    } else if (contrast2 > contrast1) {
        moreContrastImage = name2;
    } else {
        moreContrastImage = "Both images have the same contrast.";
    }

    // Return a structure with the results
    return { name1, name2, contrast1, contrast2, moreContrastImage };
}

// Method to calculate the contrast of an image
double ContrastComporator::calculateContrast(const QImage &image) {
    double meanLuminance = 0.0;  // Average luminance
    double variance = 0.0;       // Variance of luminance

    int width = image.width();
    int height = image.height();
    int pixelCount = width * height;

    // Calculate the average luminance of all pixels
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color = image.pixelColor(x, y);
            double luminance = 0.2126 * color.red() + 0.7152 * color.green() + 0.0722 * color.blue();
            meanLuminance += luminance;
        }
    }
    meanLuminance /= pixelCount;

    // Calculate the variance of luminance
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color = image.pixelColor(x, y);
            double luminance = 0.2126 * color.red() + 0.7152* color.green() + 0.0722 * color.blue();
            variance += std::pow(luminance - meanLuminance, 2);
        }
    }
    variance /= pixelCount;

    // Return the standard deviation of luminance as the contrast value
    return std::sqrt(variance);
}

QString ContrastComporator::getShortName() const {
    return "Contrast";
}

QString ContrastComporator::getFullName() const {
    return "Comparison of image contrast";
}

QString ContrastComporator::getHotkey() const {
    return "K";
}

QString ContrastComporator::getDescription() const {
    return QString("The algorithm calculates the contrast of an image based on the statistical ")
                + "analysis of pixel brightness (luminance). It does not compare brightness directly but "
                + "uses it to determine the spread of values, allowing for the assessment of differences "
                + "between light and dark areas of the image.";
}

std::shared_ptr<ComparisonResultVariant> ContrastComporator::compare(
                                                            const ComparableImage& first,
                                                            const ComparableImage& second
                                                            )
{
    auto result = compareImages(first.getImage(),
                                first.getName(),
                                second.getImage(),
                                second.getName()
                                );
    QString html = ContrastComporator::formatResultToHtml(result);
    return std::make_shared<ComparisonResultVariant>(html);
}

QString ContrastComporator::formatResultToHtml(const ContrastComparisonResult& result) {
    QString html;
    html += QString("<h2 style=\"line-height: 2;\">%1</h2>").arg(getFullName());
    html += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">";
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(result.image1Name)
                .arg(result.contrast1);
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(result.image2Name)
                .arg(result.contrast2);
    html += QString("<tr><td colspan=\"2\" align=\"center\"><b>The image with more contrast:"
                    " <font color=\"green\">%1</font></b></td></tr>").arg(result.moreContrastImageName);
    html += "</table>";
    html += "<br /><br />";
    html += "The range of coefficient values is [0, ∞).<br/>";
    html += QString("The minimum contrast value is 0, which corresponds to an image with ")
            + "completely uniform brightness for all pixels (e.g., a solid color).";

    return html;
}
