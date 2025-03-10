#include "comparisioninteractor.h"

#include <QtCore/qdir.h>
#include <savefileinfo.h>
#include <comparisionmanager.h>
#include <qfileinfo.h>

ComparisionInteractor::ComparisionInteractor(AMainWindowCallbacks *callbacks) :
    callbacks(callbacks)
{

}

void ComparisionInteractor::onImageComporatorShouldBeCalled(QVariant callerData) {
    if (!callerData.isValid() || callerData.isNull()) {
        return;
    }

    QString comporatorName = callerData.toString();
    auto comporator = ComparisionManager::instance()->findComporator(comporatorName);
    if (comporator == nullptr) {
        return;
    }
    QList<QString> filesPath;
    filesPath.append(firstImagePath);
    filesPath.append(secondImagePath);
    auto result = comporator->compare(filesPath);

    if (result->type() == ComparisionResultVariantType::Image) {
        QImage imageResult = result->imageResult();
        QPixmap pixmap = QPixmap::fromImage(imageResult);
        if (!pixmap.isNull()) {
            callbacks->onComparisonImagesLoaded(pixmap, comporator->name());
        }
    }
    else if (result->type() == ComparisionResultVariantType::String) {
        QString stringResult = result->stringResult();
        if (stringResult.isNull() || stringResult.isEmpty()) {
            return;
        }
        callbacks->onComparisonTextLoaded(stringResult);
    }
}

void ComparisionInteractor::clear() {
    firstImagePath = {};
    secondImagePath = {};
    firstImage = {};
    secondImage = {};
    comparisionImage = {};
}

void ComparisionInteractor::loadImagesBeingCompared(QString &Image1Path, QString &Image2Path) {

    firstImagePath = Image1Path;
    secondImagePath = Image2Path;
    firstImage = QPixmap();
    secondImage= QPixmap();

    if (!validateFilePath(firstImagePath) || !validateFilePath(secondImagePath)) {
        clear();
        throw std::runtime_error("Error: Both images must be selected!");
    }

    bool isLoaded1 = firstImage.load(firstImagePath);
    bool isLoaded2 = secondImage.load(secondImagePath);

    if (firstImage.size() != firstImage.size()) {
        clear();
        throw std::runtime_error("Error: Images must have the same resolution!");
    }

    if (!isLoaded1 || !isLoaded2) {
        clear();
        throw std::runtime_error("Error: Unable to load images!");
    }

    callbacks->onImagesBeingComparedLoaded(firstImage, firstImagePath, secondImage, secondImagePath, false, {});
}

bool ComparisionInteractor::validateFilePath(const QString &filePath) {

    if (filePath.isEmpty()) {
        return false;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return false;
    }

    return true;
}

void ComparisionInteractor::saveImage(SaveImageInfo info) {
    if (info.saveImageInfoType == SaveImageInfoType::None || info.image.isNull()) {
        return;
    }

    QFileInfo file1 = QFileInfo(firstImagePath);
    QFileInfo file2 = QFileInfo(secondImagePath);
    QString file1Name = file1.baseName();
    QString file2Name = file2.baseName();
    QDir defaultDir = file1.absoluteDir();
    QString defaultExtention = QString(".") + file1.suffix();
    QString fileName, fullPath;

    switch (info.saveImageInfoType) {
    case SaveImageInfoType::FirstImage:
        callbacks->saveImageAs(info.image, firstImagePath);
        break;
    case SaveImageInfoType::SecondImage:
        callbacks->saveImageAs(info.image, secondImagePath);
        break;
    case SaveImageInfoType::FirstImageArea:
        fileName = file1Name + "_area" + defaultExtention;
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImageAs(info.image, fullPath);
        break;
    case SaveImageInfoType::SecondImageArea:
        fileName = file2Name + "_area" + defaultExtention;
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImageAs(info.image, fullPath);
        break;
    case SaveImageInfoType::ComparisonImage:
        fileName = QString("%1_vs_%2_comparison%3")
                       .arg(file1Name, file2Name, defaultExtention);
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImageAs(info.image, fullPath);
        break;
    case SaveImageInfoType::ComparisonImageArea:
        fileName = QString("%1_vs_%2_areas_comparison%3")
                       .arg(file1Name, file2Name, defaultExtention);
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImageAs(info.image, fullPath);
        break;
    default:
        break;
    }
}

void ComparisionInteractor::onDisplayedRgbChannelsChanged(RgbChannels channels, ImageGeometry imageGeometry) {

    if (channels == RgbChannels::All) {
            callbacks->onImagesBeingComparedLoaded(firstImage,
                                                   firstImagePath,
                                                   secondImage,
                                                   secondImagePath,
                                                   true,
                                                   imageGeometry);
    } else {
        QPixmap oneChannelImage1 = extractChannel(firstImage, channels);
        QPixmap oneChannelImage2 = extractChannel(secondImage, channels);
        callbacks->onImagesBeingComparedLoaded(oneChannelImage1,
                                               firstImagePath,
                                               oneChannelImage2,
                                               secondImagePath,
                                               true,
                                               imageGeometry);
    }
}


QPixmap ComparisionInteractor::extractChannel(const QPixmap& pixmap, RgbChannels channels) {
    // Convert QPixmap to QImage for pixel manipulation
    QImage image = pixmap.toImage();

    // Create a new QImage to store the red channel
    QImage oneChannelImage(image.size(), QImage::Format_ARGB32);

    // Iterate over each pixel
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            // Extract the original pixel
            QRgb pixel = image.pixel(x, y);

            if (channels == RgbChannels::R) {
                int red = qRed(pixel);
                QRgb newPixel = qRgba(red, 0, 0, qAlpha(pixel));
                oneChannelImage.setPixel(x, y, newPixel);
            } else if (channels == RgbChannels::G) {
                int green = qGreen(pixel);
                QRgb newPixel = qRgba(0, green, 0, qAlpha(pixel));
                oneChannelImage.setPixel(x, y, newPixel);
            } else if (channels == RgbChannels::B) {
                int blue = qBlue(pixel);
                QRgb newPixel = qRgba(0, 0, blue, qAlpha(pixel));
                oneChannelImage.setPixel(x, y, newPixel);
            } else {
                throw new std::runtime_error("Error: An incorrect RGB channel was requested.");
            }
        }
    }

    // Convert the modified QImage back to QPixmap
    return QPixmap::fromImage(oneChannelImage);
}






