#include "imagefileshandler.h"

#include <QStringList>
#include <QtCore/qdir.h>
#include <qfileinfo.h>
#include <quuid.h>
#include <business/recentfilesmanager.h>
#include <business/imageanalysis/imageprocessinginteractor.h>
#include <data/storage/filedialoghandler.h>
#include <domain/valueobjects/images.h>
#include <business/utils/imagesinfo.h>
#include <business/validation/imagevalidationrulesfactory.h>
#include <data/storage/stb_image.h>

// The user can drag and drop one or two images into the application window.
// In the case of a single image, it will simply open for viewing; only filters
// will be available (but not comparators). In the case of two images, the user
// will be able to compare them; both filters and comparators will be available.
ImageHolderPtr ImageFilesHandler::openImages(const QList<QUrl> &urls) {
    if (urls.size() == 0 || urls.size() > 2) {
        QString err = QString("Drag and drop one or two images here.");
        throw std::runtime_error(err.toStdString());
    }
    if (urls.size() == 2 && urls[0].isLocalFile() && urls[1].isLocalFile()) {
        const QString firstImagePath = urls[0].toLocalFile();
        const QString secondImagePath = urls[1].toLocalFile();
        return openImages(firstImagePath, secondImagePath);
    } else if (urls.size() == 1 && urls[0].isLocalFile()) {
        const QString imagePath = urls[0].toLocalFile();
        return openImage(imagePath);
    }
    throw std::runtime_error("Incorrect path to image.");
}

ImageHolderPtr ImageFilesHandler::openImages() {
    FileDialogHandler handler;
    OptionalStringPair twoUserImagePaths = handler.getUserOpenTwoImagePaths("");
    if (!twoUserImagePaths) {
        return nullptr;  // operation is canceled by the user, this is not an error indicator
    }
    return openImages(twoUserImagePaths.value().first,
                      twoUserImagePaths.value().second
                      );
}

ImageHolderPtr ImageFilesHandler::openImage() {
    FileDialogHandler handler;
    std::optional<QString> userImagePath = handler.getUserOpenImagePath("");
    if (!userImagePath) {
        return nullptr;  // operation is canceled by the user, this is not an error indicator
    }
    return openImage(userImagePath.value());
}

ImageHolderPtr ImageFilesHandler::openImages(const QString &firstImagePath,
                                             const QString &secondImagePath
                                             )
{
    QPixmap firstImage = coreOpenImage(firstImagePath);
    QPixmap secondImage = coreOpenImage(secondImagePath);
    ImageHolderPtr imageHolder = std::make_shared<ImageHolder>(firstImage,
                                                               firstImagePath,
                                                               secondImage,
                                                               secondImagePath
                                                               );
    validateImages(imageHolder);
    return imageHolder;
}

ImageHolderPtr ImageFilesHandler::openImage(const QString &imagePath) {
    QPixmap image = coreOpenImage(imagePath);
    ImageHolderPtr imageHolder = std::make_shared<ImageHolder>(image, imagePath);
    validateImages(imageHolder);
    return imageHolder;
}

QPixmap ImageFilesHandler::coreOpenImage(const QString &imagePath) {
    int width, height, channels;
    unsigned char* data = stbi_load(imagePath.toStdString().c_str(),
                                    &width,
                                    &height,
                                    &channels,
                                    4
                                    );
    if (!data) {
        QString error = QString("Unable to open " +
                                imagePath + ". The image format might "
                                "not be supported, or the file does not exist."
                                );
        throw std::runtime_error(error.toStdString());
    }
    QImage image(reinterpret_cast<const uchar*>(data),
                 width,
                 height,
                 QImage::Format_RGBA8888
                 );

    QPixmap pixmap = QPixmap::fromImage(image.copy());
    stbi_image_free(data);
    return pixmap;
}

void ImageFilesHandler::validateImages(ImageHolderPtr images) {
    auto validationRules = ImageValidationRulesFactory::createImageFormatValidator(images);
    auto error = validationRules->isValid();
    if (error != std::nullopt) {
        std::string strError = error->toStdString();
        throw std::runtime_error(strError);
    }
}

QString ImageFilesHandler::saveImageAsTemporary(const QPixmap &image) {
    auto extentionValidator = ImageValidationRulesFactory::createImageExtensionsInfoProvider();
    QString ext = extentionValidator->getDeafaultSaveExtension(true);
    QString uniqueName = QUuid::createUuid().toString(QUuid::WithoutBraces) + ext;
    QString tempDir = QDir::tempPath();
    QString filePath = QDir(tempDir).filePath(uniqueName);
    if (image.save(filePath)) {
        return filePath;
    }
    throw std::runtime_error("Unable to save the image in the Temp directory.");
}

std::optional<QString> ImageFilesHandler::saveImageAs(const SaveImageInfo &saveImageInfo,
                                                      const ImageHolderPtr imageHolder
                                                      )
{
    if (saveImageInfo.mSaveImageInfoType == SaveImageInfoType::None ||
        saveImageInfo.mImage.isNull() ||
        imageHolder == nullptr
        )
    {
        throw std::runtime_error("ImageHolderPtr is nullptr or bad SaveImageInfoType"); // for qDebug()
    }

    ImagesInfo imagesInfo{imageHolder};

    auto extentionValidator = ImageValidationRulesFactory::createImageExtensionsInfoProvider();
    QString ext = extentionValidator->getDeafaultSaveExtension(true);

    const QString &firstImageName = imagesInfo.getFirstImageBaseName();
    const QString &secondImageName = imagesInfo.getSecondImageBaseName();
    QDir firstImageDir{imagesInfo.getFirstImageDir()};
    QDir secondImageDir{imagesInfo.getSecondImageDir()};
    QString fileName, fullPath;

    switch (saveImageInfo.mSaveImageInfoType) {
    case SaveImageInfoType::FirstImage:
        fileName = QString("%1_copy%2").arg(firstImageName, ext);
        fullPath = firstImageDir.filePath(fileName);
        break;
    case SaveImageInfoType::SecondImage:
        fileName = QString("%1_copy%2").arg(secondImageName, ext);
        fullPath = secondImageDir.filePath(fileName);
        break;
    case SaveImageInfoType::FirstImageArea:
        fileName = QString("%1_area%2").arg(firstImageName, ext);
        fullPath = firstImageDir.filePath(fileName);
        break;
    case SaveImageInfoType::SecondImageArea:
        fileName = QString("%1_area%2").arg(secondImageName, ext);
        fullPath = firstImageDir.filePath(fileName);
        break;
    case SaveImageInfoType::ComparisonImage:
        fileName = QString("%1_vs_%2_comparison%3")
                       .arg(firstImageName, secondImageName, ext);
        fullPath = firstImageDir.filePath(fileName);
        break;
    case SaveImageInfoType::ComparisonImageArea:
        fileName = QString("%1_vs_%2_area_comparison%3")
                       .arg(firstImageName, secondImageName, ext);
        fullPath = firstImageDir.filePath(fileName);
        break;
    default:
        break;
    }

    bool isSaved = false;
    FileDialogHandler saveFileDialog{};
    auto savePath = saveFileDialog.getUserSaveImagePath(fullPath);
    if (!savePath) {
        return std::nullopt;
    }
    isSaved = saveImageInfo.mImage.save(savePath.value());
    if (!isSaved) {
        throw std::runtime_error("QPixmap::save(QString) returns false."); // for qDebug()
    }
    return fullPath;
}
