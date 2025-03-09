#include <QPainter>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

#include "imagecomparator.h"

ImageComparator::ImageComparator(const QString &image1Path, const QString &image2Path)
        : image1Path(image1Path), image2Path(image2Path) {

}

// Method to compare images and save the result
QString ImageComparator::compareImagesAndSaveResultAsImage() {
    // Load images
    QImage img1(image1Path);
    QImage img2(image2Path);

    if (img1.isNull() || img2.isNull()) {
        throw std::runtime_error("Failed to load one of the images.");
    }

    // Check if image sizes match
    if (img1.size() != img2.size()) {
        throw std::runtime_error("Image sizes do not match!");
    }

    // Create a resulting image (copy of the first image)
    QImage resultImg = img1.copy();
    QPainter painter(&resultImg);

    // Add a semi-transparent layer
    painter.setOpacity(0.5);
    painter.drawImage(0, 0, img1);
    painter.setOpacity(1.0);

    // Compare pixels and highlight differences in red
    for (int y = 0; y < img1.height(); ++y) {
        for (int x = 0; x < img1.width(); ++x) {
            if (img1.pixel(x, y) != img2.pixel(x, y)) {
                resultImg.setPixelColor(x, y, QColor(255, 0, 0, 255)); // Red color
            }
        }
    }

    painter.end();

    // Save the resulting image
    QString resultPath = saveResultImage(resultImg);
    return resultPath;
}

// Method to save the resulting image
QString ImageComparator::saveResultImage(const QImage &resultImg) {
    QFileInfo fileInfo1(image1Path);
    QFileInfo fileInfo2(image2Path);

    // Get file names without extensions
    QString image1Name = fileInfo1.completeBaseName();
    QString image2Name = fileInfo2.completeBaseName();

    // Generate the name for the resulting file
    QString resultFileName = QString("%1_vs_%2_comparison.png").arg(image1Name, image2Name);

    // Path to save the result (in the folder of the first image)
    QDir parentDir = fileInfo1.absoluteDir();
    QString resultPath = parentDir.filePath(resultFileName);

    // Save the image
    if (!resultImg.save(resultPath)) {
        throw std::runtime_error("Failed to save the resulting image.");
    }

    qDebug() << "Result saved at:" << resultPath;
    return resultPath;
}
