#include "appleiconmaker.h"

#include <QtGui/qpainter.h>
#include <qdir.h>

AppleIconMaker::AppleIconMaker() {}


QString AppleIconMaker::getShortName() const {
    return "Create Icons For iOS Apps";
}

QString AppleIconMaker::getHotkey() const {
    return "I";
}

QString AppleIconMaker::getDescription() const {
    return "It's not exactly a filter, but a useful tool "
           "that can be used to create icons for an iOS app.";
}

QString AppleIconMaker::getFullName() const {
    return "Create Icons For iOS Apps";
}

QImage AppleIconMaker::filter(const QImage &image) {
    struct IconSize {
        int size;
        QString filename;
    };

    QString outputPath = QDir::homePath();
    outputPath += QString(QDir::separator()) + "TwinPix";

    QList<IconSize> iconSizes = {
        {20, "AppIcon-20x20@1x.png"},
        {40, "AppIcon-20x20@2x.png"},
        {60, "AppIcon-20x20@3x.png"},
        {29, "AppIcon-29x29@1x.png"},
        {58, "AppIcon-29x29@2x.png"},
        {87, "AppIcon-29x29@3x.png"},
        {40, "AppIcon-40x40@1x.png"},
        {80, "AppIcon-40x40@2x.png"},
        {120, "AppIcon-40x40@3x.png"},
        {120, "AppIcon-60x60@2x.png"},
        {180, "AppIcon-60x60@3x.png"},
        {76, "AppIcon-76x76@1x.png"},
        {152, "AppIcon-76x76@2x.png"},
        {167, "AppIcon-83.5x83.5@2x.png"},
        {1024, "AppIcon-1024x1024@1x.png"} // App Store icon
    };

    QDir outputDir(outputPath);
    QString folderName = "iOS_App_Icons";
    QString targetPath = outputDir.filePath(folderName);
    if (!outputDir.exists(targetPath)) {
        if (!outputDir.mkpath(targetPath)) {
            QString error = QString("Unable to create the directory: %1")
                                .arg(targetPath);
            throw std::runtime_error(error.toStdString());
        }
    }

    for (const IconSize &icon : iconSizes) {
        QImage resizedImage = image.scaled(icon.size,
                                           icon.size,
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation
                                           );
        QString outputFilePath = QDir(targetPath).filePath(icon.filename);
        if (!resizedImage.save(outputFilePath)) {
            QString error = QString("Unable to create the icon: %1")
            .arg(outputFilePath);
            throw std::runtime_error(error.toStdString());
        }
    }

    QString text = QString("All icons successfully saved in folder:\n%1").arg(targetPath);

    return createImageWithText(image, text);
}

QImage AppleIconMaker::createImageWithText(const QImage &sourceImage, const QString &text) {

    QImage newImage(sourceImage.size(), QImage::Format_ARGB32);
    newImage.fill(Qt::white);

    QPainter painter(&newImage);
    painter.setRenderHint(QPainter::Antialiasing);

    QFont font("Arial", 30);
    font.setBold(true);
    painter.setFont(font);

    painter.setPen(Qt::black);

    QRect textRect = QRect(0, 0, newImage.width(), newImage.height());

    painter.drawText(textRect, Qt::AlignCenter, text);

    painter.end();

    return newImage;
}

