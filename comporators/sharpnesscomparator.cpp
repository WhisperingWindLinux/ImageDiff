#include "sharpnesscomparator.h"

#include "sharpnesscomparator.h"

#include <QFileInfo>
#include <QDebug>
#include <QtMath>

// Compare the two images and return a structure with the results
SharpnessComparisonResult SharpnessComparator::compareImages(QImage image1,
                                                             QString name1,
                                                             QImage image2,
                                                             QString name2)
{
    double sharpness1 = calculateSharpness(image1);
    double sharpness2 = calculateSharpness(image2);

    QString sharperImage;
    if (sharpness1 > sharpness2) {
        sharperImage = name1;
    } else if (sharpness2 > sharpness1) {
        sharperImage = name2;
    } else {
        sharperImage = "Equal";
    }

    return {name1, name2, sharpness1, sharpness2, sharperImage};
}


// Calculate the sharpness of an image using gradient magnitude
double SharpnessComparator::calculateSharpness(const QImage& image) {
    double totalGradient = 0.0;
    int pixelCount = 0;

    for (int y = 1; y < image.height() - 1; ++y) {
        for (int x = 1; x < image.width() - 1; ++x) {
            QColor center(image.pixel(x, y));
            QColor right(image.pixel(x + 1, y));
            QColor bottom(image.pixel(x, y + 1));

            // Calculate gradients in X and Y directions
            double gradientX = qAbs(center.valueF() - right.valueF());
            double gradientY = qAbs(center.valueF() - bottom.valueF());

            // Total gradient magnitude
            totalGradient += qSqrt(gradientX * gradientX + gradientY * gradientY);
            ++pixelCount;
        }
    }

    return (pixelCount > 0) ? (totalGradient / pixelCount) : 0.0;
}

QString SharpnessComparator::getShortName() const {
    return "Sharpness";
}

QString SharpnessComparator::getFullName() const {
    return "Comparison of image sharpness";
}

QString SharpnessComparator::getHotkey() const {
    return "H";
}

QString SharpnessComparator::getDescription() const {
    return QString("This algorithm compares the sharpness of two images by calculating ")
           + "their sharpness values based on the gradient magnitude of pixel intensity "
           + "differences in both the horizontal and vertical directions."
           + " Where higher values indicate a more sharper image.";
}

std::shared_ptr<ComparisonResultVariant> SharpnessComparator::compare(ComparableImage first, ComparableImage second) {
    auto result = compareImages(first.getImage(),
                                first.getName(),
                                second.getImage(),
                                second.getName());

    QString html = SharpnessComparator::formatResultToHtml(result);
    return std::make_shared<ComparisonResultVariant>(html);
}

QString SharpnessComparator::formatResultToHtml(const SharpnessComparisonResult& result) {

    QString html;
    html += QString("<h2 style=\"line-height: 2;\">%1</h2>").arg(getFullName());
    html += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">";
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(result.name1)
                .arg(result.sharpness1);
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(result.name2)
                .arg(result.sharpness2);
    html += QString("<tr><td colspan=\"2\" align=\"center\"><b>The more sharper image is "
                    "</b> <font color=\"green\">%1</font></td></tr>").arg(result.sharperImage);
    html += "</table>";
    html += "<br /><br />";
    html += QString("The range of coefficient values is approx [0.0, 1.414]. ")
            + "Higher values indicate a more sharper image.";

    return html;
}
