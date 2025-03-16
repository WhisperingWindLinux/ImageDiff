#include "imageproximitytoorigincomparator.h"

#include <QImage>
#include <QColor>
#include <QString>
#include <QDebug>
#include <qfileinfo.h>

ImageProximityToOriginResult ImageProximityToOriginComparator::compareImages(QImage image1,
                                                        QImage image2,
                                                        QString name1,
                                                        QString name2,
                                                        QImage originalImage
                                                        )
{
    // Ensure all images have the same size
    if (image1.size() != image2.size() || image1.size() != originalImage.size()) {
        throw runtime_error("Error: Images have different sizes. Comparison is not possible.");
    }

    // Calculate the total difference for each image compared to the original
    int totalDifference1 = calculateTotalDifference(image1, originalImage);
    int totalDifference2 = calculateTotalDifference(image2, originalImage);

    QString strResult;
    if (totalDifference1 < totalDifference2) {
        strResult = QString(name1) + " is closer to the original image.";
    } else if (totalDifference1 > totalDifference2) {
        strResult = QString(name2) + " is closer to the original image.";
    } else {
        strResult = "Both images are equally close to the original image.";
    }

    return {name1, name2, totalDifference1, totalDifference2, strResult };
}


qint64 ImageProximityToOriginComparator::calculateTotalDifference(const QImage &image, const QImage &originalImage) {
    int width = image.width();
    int height = image.height();
    qint64 totalDifference = 0;

    // Loop through each pixel in the images
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color1 = image.pixelColor(x, y);
            QColor colorOriginal = originalImage.pixelColor(x, y);

            // Calculate the squared difference for each RGB component
            int diffR = color1.red() - colorOriginal.red();
            int diffG = color1.green() - colorOriginal.green();
            int diffB = color1.blue() - colorOriginal.blue();

            // Sum up the squared differences (Euclidean distance squared)
            totalDifference += diffR * diffR + diffG * diffG + diffB * diffB;
        }
    }
    return totalDifference;
}

QList<Property> ImageProximityToOriginComparator::getDefaultProperties() const {

    Property filePathProperty = Property::createFilePathProperty("Path to original image",
                                                                getDescription(),
                                                                 "");
    return { filePathProperty };

}

void ImageProximityToOriginComparator::setProperties(QList<Property> properties) {

    #ifdef QT_DEBUG
    if (!pathToOriginalImage.isEmpty()) {
        throw runtime_error("Error: Unabel to load the image.");
    }
    #endif

    if (properties.size() != 1) {
        return;
    }
    QString filePath = properties[0].getFilePath();
    if (filePath.isEmpty()) {
        return;
    }
    QFileInfo filePathInfo {filePath};
    if (!filePathInfo.isFile() || !filePathInfo.exists()) {
        return;
    }

    pathToOriginalImage = filePath;
}

void ImageProximityToOriginComparator::reset() {
    pathToOriginalImage = "";
}

QString ImageProximityToOriginComparator::getShortName() const {
    return "Proximity To Original";
}

QString ImageProximityToOriginComparator::getFullName() const {
    return "Proximity to an original image";
}

bool ImageProximityToOriginComparator::isPartOfAutoReportingToolbox() {
    return false;
}

QString ImageProximityToOriginComparator::getHotkey() const {
    return "Y";
}

QString ImageProximityToOriginComparator::getDescription() const {
    return QString("This algorithm calculates the total difference between two ")
                    + "images using the squares of the differences in color "
                    + "component values (R, G, B) of each pixel. "
                    + "According to this criterion, the closeness "
                    + "of each of the two images to the original is evaluated.";
}

std::shared_ptr<ComparisonResultVariant> ImageProximityToOriginComparator::compare(
                                                                        ComparableImage first,
                                                                        ComparableImage second
                                                                        )
{

    QImage originalImage;
    originalImage.load(pathToOriginalImage);

    if (originalImage.isNull()) {
        throw runtime_error("Error: Unable to laod the original image.");
    }

    auto result = compareImages(first.getImage(),
                                second.getImage(),
                                first.getName(),
                                second.getName(),
                                originalImage
                                );

    QString html = ImageProximityToOriginComparator::formatResultToHtml(result);
    std::shared_ptr<ComparisonResultVariant> resultVariant = std::make_shared<ComparisonResultVariant>(html);
    return resultVariant;
}

QString ImageProximityToOriginComparator::formatResultToHtml(const ImageProximityToOriginResult &result) {
    QLocale locale = QLocale::system();
    QString formattedTotalDiff1 = locale.toString(result.totalDifference1);
    QString formattedTotalDiff2 = locale.toString(result.totalDifference2);

    QString html;
    html += QString("<h2 style=\"line-height: 2;\">%1</h2>").arg(getFullName());
    html += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\">";
    html += QString("<tr><td>The difference between %1 and the original image</td><td>%2</td></tr>")
                .arg(result.image1Name)
                .arg(formattedTotalDiff1);
    html += QString("<tr><td>The difference between %1 and the original image</td><td>%2</td></tr>")
                .arg(result.image2Name)
                .arg(formattedTotalDiff2);
    html += QString("<tr><td colspan=\"2\" align=\"center\"><b>"
                    " <font color=\"green\">%1</font></b></td></tr>").arg(result.resultDescription);
    html += "</table>";
    html += "<br /><br />";
    html += QString("The result of the function indicates ")
            + "the degree of difference between two images. "
            + "The higher the value, the greater the differences between them.";

    return html;
}
