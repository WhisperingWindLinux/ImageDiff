#include "imageproximitytoorigincomparator.h"

#include <QImage>
#include <QColor>
#include <QString>
#include <QDebug>
#include <cmath>
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


int ImageProximityToOriginComparator::calculateTotalDifference(const QImage &image, const QImage &originalImage) {
    int width = image.width();
    int height = image.height();
    int totalDifference = 0;

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

QString ImageProximityToOriginComparator::formatResultToHtml(const ImageProximityToOriginResult &result) {
    QString html;
    html += "<html>";
    html += "<head><style>";
    html += "table { border-collapse: collapse; width: 100%; }";
    html += "th, td { border: 1px solid black; padding: 8px; text-align: left; }";
    html += "th { background-color: #f2f2f2; }";
    html += "</style></head>";
    html += "<body>";

    html += "<h2>Image Proximity Comparison Results</h2>";
    html += "<br />";
    html += "<table>";
    html += "<tr><th>Parameter</th><th>Value</th></tr>";
    html += QString("<tr><td>Image 1 Name</td><td>%1</td></tr>").arg(result.image1Name);
    html += QString("<tr><td>Image 2 Name</td><td>%1</td></tr>").arg(result.image2Name);
    html += QString("<tr><td>Total Difference (Image 1)</td><td>%1</td></tr>").arg(result.totalDifference1);
    html += QString("<tr><td>Total Difference (Image 2)</td><td>%1</td></tr>").arg(result.totalDifference2);
    html += QString("<tr><td>Result Description</td><td>%1</td></tr>").arg(result.resultDescription);
    html += "</table>";

    html += "<br />";
    html += "</body>";
    html += "</html>";

    return html;
}

QList<Property> ImageProximityToOriginComparator::getDefaultProperties() const {
    QString description = QString("The algorithm assumes that the two images opened in the application ") +
        "are screenshots taken using an HDMI recorder. And the original image refers " +
                          "to either a screenshot taken using the operating system's tools or the original picture.";

    Property filePathProperty = Property::createFilePathProperty("Path to original image",
                                                                 description,
                                                                 "");
    return { filePathProperty };

}

void ImageProximityToOriginComparator::setProperties(QList<Property> properties) {

    #ifdef QT_DEBUG
    if (!pathToOriginalImage.isEmpty()) {
        throw runtime_error(
            "ImageProximityToOriginComparator: pathToOriginalImage is not empty!"
            );
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

QString ImageProximityToOriginComparator::name() {
    return "Compare Two Images To Original One";
}

QString ImageProximityToOriginComparator::hotkey() {
    return "Y";
}

QString ImageProximityToOriginComparator::description() {
    return QString("Compare the proximity of two images to an original image. ") +
        "The algorithm assumes that the two images opened in the application " +
        "are screenshots taken using an HDMI recorder. And the original image refers " +
        "to either a screenshot taken using the operating system's tools or the original picture.";
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
