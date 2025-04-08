#include "linernonlinerdifferencecomparator.h"
#include <QtCore/qmath.h>
#include <business/imageanalysis/comporators/helpers/mathhelper.h>


LinerNonLinerDifferenceComparator::LinerNonLinerDifferenceComparator()
    : mThreshold(5.0)
{
}

QString LinerNonLinerDifferenceComparator::getShortName() const {
    return "Linear/non-linear difference between images";
}

QString LinerNonLinerDifferenceComparator::getHotkey() const {
    return "L";
}

QString LinerNonLinerDifferenceComparator::getDescription() const {
    return "This algorithm checks whether the difference between two graphics "
           "cards is linear and consistent for all pixels. To achieve this,"
           " a statistical analysis of the difference between two images is "
           "performed. If the difference between corresponding pixels of the"
           " two images is not constant or predictable (e.g., depends on "
           "brightness or other factors), it indicates non-linearity.";
}

QString LinerNonLinerDifferenceComparator::getFullName() const {
    return getShortName();
}

LinerNonLinerComparisonResult LinerNonLinerDifferenceComparator::compareImages(const QImage &image1,
                                                                                const QString &,
                                                                                const QImage &image2,
                                                                                const QString &
                                                                                )
{
    int width = image1.width();
    int height = image1.height();

    QVector<double> differences;
    QVector<double> brightnessValues;

    // Iterate through all pixels of the images
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Extract pixels from both images
            QRgb pixel1 = image1.pixel(x, y);
            QRgb pixel2 = image2.pixel(x, y);

            // Brightness as the average value of R, G, B channels
            double brightness1 = (qRed(pixel1) + qGreen(pixel1) + qBlue(pixel1)) / 3.0;
            double brightness2 = (qRed(pixel2) + qGreen(pixel2) + qBlue(pixel2)) / 3.0;

            // Average difference between pixels (based on brightness)
            double difference = brightness2 - brightness1;

            // Store brightness and difference for analysis
            brightnessValues.append(brightness1);
            differences.append(difference);
        }
    }

    // Calculate the mean and standard deviation of the differences
    double meanDifference = 0;
    for (double diff : differences) {
        meanDifference += diff;
    }
    meanDifference /= differences.size();

    double variance = 0;
    for (double diff : differences) {
        variance += qPow(diff - meanDifference, 2);
    }
    variance /= differences.size();
    double stdDeviation = qSqrt(variance);

    return { meanDifference, mThreshold, stdDeviation };
}

QString LinerNonLinerDifferenceComparator::formatResultToHtml(const LinerNonLinerComparisonResult &result) {

    QString html;

    QString r1 = "Mean difference";
    QString r2 = "Standard deviation of the difference";
    QString r3 = "Threshold";

    html += QString("<h2 style=\"line-height: 2;\">%1</h2>").arg(getFullName());
    html += R"(<table border="1" cellspacing="0" cellpadding="5">)";
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(r1)
                .arg(result.meanDifference, 0, 'g', 2);
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(r3)
                .arg(result.threshold);
    html += QString("<tr><td>%1</td><td>%2</td></tr>")
                .arg(r2)
                .arg(result.stdDeviation, 0, 'g', 2);


    // Check linearity: if the standard deviation is small, consider the difference linear
    if (result.stdDeviation < result.threshold) {
        html += R"(<tr><td colspan="2" align="center">
                   <font color="green">The difference between the graphics cards is
                   linear and consistent for all pixels</font></td></tr>)";
    } else {
        html += R"(<tr><td colspan="2" align="center">
                   <font color="red">The difference between the graphics cards is
                   non-linear or inconsistent</font></td></tr>)";
    }
    html += "</table>";
    html += "<br /><br />";

    html += R"(<b>Mean Difference</b>: the average difference in brightness between corresponding
               pixels of two images. A value close to 0 means the images are very similar
               in brightness. A positive value indicates that the second image is generally
               brighter than the first. A negative value indicates that the second image is generally
               darker than the first.<br/><b>Threshold</b>: a predefined value used to determine
               if differences are small and consistent or large and inconsistent. If the standard deviation of
               the differences is smaller than this threshold, the differences are considered to be small
               and consistent across all pixels. If the standard deviation exceeds this threshold, it
               suggests that there are significant variations in how the two images differ, which may
               indicate non-linear behavior or inconsistencies.<br/><b>Standard
               Deviation</b>: measures how much pixel brightness differences vary; a low value
               indicates consistent differences, while a high value suggests variability.A small standard
               deviation means that most of the pixel differences are close to the mean difference, indicating
               consistent behavior across the image. A large standard deviation means that there is significant
               variability in how different pixels compare, suggesting inconsistencies or non-linear behavior.
               <br/>)";

    return html;
}

ComparisonResultVariantPtr LinerNonLinerDifferenceComparator::compare(const ComparableImage &first,
                                                                       const ComparableImage &second
                                                                       )
{
    auto result = compareImages(first.getImage(),
                                first.getImageName(),
                                second.getImage(),
                                second.getImageName()
                                );

    QString html = formatResultToHtml(result);
    ComparisonResultVariantPtr resultVariant = std::make_shared<ComparisonResultVariant>(html);
    return resultVariant;
}


QList<Property> LinerNonLinerDifferenceComparator::getDefaultProperties() const {
    QString description = R"(Threshold: a predefined value (default: 5.0) used to determine
               if differences are small and consistent or large and inconsistent. If the standard deviation of
               the differences is smaller than this threshold, the differences are considered to be small
               and consistent across all pixels. If the standard deviation exceeds this threshold, it
               suggests that there are significant variations in how the two images differ, which may
               indicate non-linear behavior or inconsistencies.)";

    auto prop1 = Property::createRealProperty("Threshold", description, 5.0, 0, 255);

    return { prop1 };
}

void LinerNonLinerDifferenceComparator::setProperties(QList<Property> properties) {
    if (properties.size() != 1) {
        QString error = "Got an error from %1: an incorrect number of properties.";
        error = error.arg(getShortName());
        throw std::runtime_error(error.toStdString());
    }
    mThreshold = properties[0].getValue();
}

void LinerNonLinerDifferenceComparator::reset() {
    mThreshold = 5.0;
}
