#include "sharpnesscomparator.h"

#include "sharpnesscomparator.h"

#include <QFileInfo>
#include <QDebug>
#include <qstring.h>

#include <business/imageanalysis/comporators/helpers/mathhelper.h>

// Compare the two images and return a structure with the results
SharpnessComparisonResult SharpnessComparator::compareImages(const QImage &image1,
                                                             const QString &name1,
                                                             const QImage &image2,
                                                             const QString &name2
                                                             )
{
    double sharpness1 = calculateSharpness(image1);
    double sharpness2 = calculateSharpness(image2);

    return {name1, name2, sharpness1, sharpness2};
}


// Calculate the sharpness of an image using gradient magnitude
double SharpnessComparator::calculateSharpness(const QImage &image) {
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
           + " The range of coefficient values is approx [0.0, 1.414]. Where higher "
             "values indicate a more sharper image.";
}

std::shared_ptr<ComparisonResultVariant> SharpnessComparator::compare(const ComparableImage &first,
                                                                      const ComparableImage &second
                                                                      )
{
    auto result = compareImages(first.getImage(),
                                first.getName(),
                                second.getImage(),
                                second.getName()
                                );

    QString html = SharpnessComparator::formatResultToHtml(result);
    return std::make_shared<ComparisonResultVariant>(html);
}

QString SharpnessComparator::formatResultToHtml(const SharpnessComparisonResult &result) {
    QString html;
    QString formatteedPersantage;

    auto raundedResult = MathHelper::roundAndCompare(result.sharpness1, result.sharpness2, 4);

    QString sharperImage;
    double persantage = qQNaN();
    if (raundedResult.value1 > raundedResult.value2) {
        sharperImage = result.name1;
        persantage = ((raundedResult.value1 - raundedResult.value2) / raundedResult.value2) * 100.0;
    } else if (raundedResult.value2 > raundedResult.value1) {
        persantage = ((raundedResult.value2 - raundedResult.value1) / raundedResult.value1) * 100.0;
        sharperImage = result.name2;
    } else {
        sharperImage = "Equal";
    }

    if (qIsNaN(persantage)) {
        formatteedPersantage = "";
    } else if ((int)(persantage * 100) == 0) {
        formatteedPersantage = " &lt;0.01%";
    } else {
        formatteedPersantage = QString(" %1%").arg(QString::number(persantage, 'f', 2));
    }
    html += QString("<h2 style=\"line-height: 2;\">%1</h2>").arg(getFullName());
    html += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">";
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(result.name1)
                .arg(raundedResult.string1);
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(result.name2)
                .arg(raundedResult.string2);

    if (!formatteedPersantage.isEmpty()) {
        html += QString("<tr><td colspan=\"2\" align=\"center\">Image<b><font "
                        "color=\"green\"> %1</font></b> is%2 sharper</td></tr>")
                    .arg(sharperImage)
                    .arg(formatteedPersantage);
    } else {
         html += "<tr><td colspan=\"2\" align=\"center\">Equally</td></tr>";
    }
    html += "</table>";
    html += "<br /><br />";
    html += QString("The range of coefficient values is approx [0.0, 1.414]. ")
            + "Where higher values indicate a more sharper image.";

    return html;
}
