#include <QFileInfo>
#include <cmath>

#include "contrastcomporator.h"

// Method to compare the contrast of two images
ContrastComparisonResult ContrastComporator::compareImages(QImage image1,
                                                           QString name1,
                                                           QImage image2,
                                                           QString name2
                                                           )
{
    if (image1.isNull() || image2.isNull()) {
        throw std::runtime_error("Failed to load one or both images.");
    }

    // Calculate contrast for both images
    double contrast1 = calculateContrast(image1);
    double contrast2 = calculateContrast(image2);

    // Determine which image has higher contrast
    QString moreContrastImage;
    if (contrast1 > contrast2) {
        moreContrastImage = QFileInfo(name1).fileName();
    } else if (contrast2 > contrast1) {
        moreContrastImage = QFileInfo(name2).fileName();
    } else {
        moreContrastImage = "Both images have the same contrast.";
    }

    // Return a structure with the results
    return {name1, name2, contrast1, contrast2, moreContrastImage};
}

// Method to format the comparison result as HTML
QString ContrastComporator::formatResultToHtml(const ContrastComparisonResult& result) {
    // Build an HTML string to display results
    QString html;
    html += "<html><body>";
    html += "<h3>Image Contrast Comparison</h3>";
    html += "<p><b>Image 1:</b> " + result.name1 + "</p>";
    html += "<p><b>Contrast:</b> " + QString::number(result.contrast1, 'f', 2) + "</p>";
    html += "<p><b>Image 2:</b> " + result.name2 + "</p>";
    html += "<p><b>Contrast:</b> " + QString::number(result.contrast2, 'f', 2) + "</p>";
    html += "<p><b>Higher Contrast Image:</b> " + result.moreContrastImage + "</p>";
    html += "</body></html>";

    return html;
}

// Method to calculate the contrast of an image
double ContrastComporator::calculateContrast(const QImage& image) {
    double meanLuminance = 0.0;  // Average luminance
    double variance = 0.0;      // Variance of luminance

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

QString ContrastComporator::name() {
    return "Show pixels' contrast difference statistics";
}

QString ContrastComporator::hotkey() {
    return "K";
}

QString ContrastComporator::description() {
    return QString("The algorithm calculates the contrast of an image based on the statistical ")
                + "analysis of pixel brightness (luminance).It does not compare brightness directly but "
                + "uses it to determine the spread of values, allowing for the assessment of differences "
                + "between light and dark areas of the image.";
}

std::shared_ptr<ComparisonResultVariant> ContrastComporator::compare(ComparableImage first, ComparableImage second) {
    auto result = compareImages(first.getImage(),
                                first.getName(),
                                second.getImage(),
                                second.getName()
                                );
    QString html = ContrastComporator::formatResultToHtml(result);
    return std::make_shared<ComparisonResultVariant>(html);
}
