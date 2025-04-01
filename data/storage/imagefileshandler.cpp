#include "imagefileshandler.h"
#include "data/storage/stb_image.h"

#include <QStringList>
#include <QtCore/qdir.h>
#include <qfileinfo.h>
#include <business/recentfilesmanager.h>
#include <business/imageanalysis/imageprocessinginteractor.h>
#include <data/storage/savefiledialoghandler.h>
#include <domain/valueobjects/images.h>
#include <business/utils/imagesinfo.h>
#include <business/validation/imagevalidationrulesfactory.h>


ImagesPtr ImageFilesHandler::openImages(const QList<QUrl> &urls) {
    if (urls.size() != 2) {
        QString err = QString("Drag and drop two images.");
        throw std::runtime_error(err.toStdString());
    }
    if (urls[0].isLocalFile() && urls[1].isLocalFile()) {
        const QString file1 = urls[0].toLocalFile();
        const QString file2 = urls[1].toLocalFile();
        return openImages(file1, file2);
    }
    throw std::runtime_error("Incorrect path to one or both images.");
}

ImagesPtr ImageFilesHandler::openImages() {
    SaveFileDialogHandler handler;
    OptionalStringPair twoUserImagePaths = handler.getUserOpenTwoImagePaths("");
    if (!twoUserImagePaths) {
        return nullptr;
    }
    return openImages(twoUserImagePaths.value().first,
                      twoUserImagePaths.value().second
                     );
}

// The function never returns nullptr; if an error occurs,
// a runtime_error with its description will be thrown.
ImagesPtr ImageFilesHandler::openImages(const QString &image1Path, const QString &image2Path) {

    int width1, height1, channels1;
    int width2, height2, channels2;

    unsigned char* data1 = stbi_load(image1Path.toStdString().c_str(), &width1, &height1, &channels1, 4);
    if (!data1) {
        QString error = QString("Unable to open " + image1Path + ". The image format might not be supported, "
                                                                 "or the file does not exist.");
        throw std::runtime_error(error.toStdString());
    }

    unsigned char* data2 = stbi_load(image2Path.toStdString().c_str(), &width2, &height2, &channels2, 4);
    if (!data2) {
        stbi_image_free(data1);
        QString error = QString("Unable to open " + image2Path + ". The image format might not be supported, "
                                                                 "or the file does not exist.");
        throw std::runtime_error(error.toStdString());
    }


    QImage image1(reinterpret_cast<const uchar*>(data1),
                  width1,
                  height1,
                  QImage::Format_RGBA8888
                  );

    QImage image2(reinterpret_cast<const uchar*>(data2),
                  width2,
                  height2,
                  QImage::Format_RGBA8888
                  );


    QPixmap pixmap1 = QPixmap::fromImage(image1.copy());
    QPixmap pixmap2 = QPixmap::fromImage(image2.copy());

    stbi_image_free(data1);
    stbi_image_free(data2);

    auto images = std::make_shared<Images>(pixmap1, pixmap2, image1Path, image2Path);

    validateImages(images);

    return images;
}

void ImageFilesHandler::validateImages(ImagesPtr images) {
    auto validationRules = ImageValidationRulesFactory::createImageFormatValidator(images);
    auto error = validationRules->isValid();
    if (error == std::nullopt) {
        return; // images are valid
    }
    std::string strError = error->toStdString();
    throw std::runtime_error(strError);
}

std::optional<FileSaveResult> ImageFilesHandler::saveImageAs(const SaveImageInfo &saveImageInfo,
                                              const ImagesPtr images
                                             )
{
    if (saveImageInfo.saveImageInfoType == SaveImageInfoType::None ||
        saveImageInfo.image.isNull() ||
        images == nullptr
        )
    {
        return std::make_optional<FileSaveResult>(false, QString("An internal error occurred: ")
                                                         + "the app is trying to save an "
                                                         + "image of an unsupported type "
                                                         + "or an empty image.");
    }

    ImagesInfo imagesInfo { images };

    auto extentionValidator = ImageValidationRulesFactory::createImageExtensionValidator();
    QString ext = extentionValidator->getDeafaultSaveExtension(true);

    const QString &file1Name = imagesInfo.getFirstImageBaseName();
    const QString &file2Name = imagesInfo.getSecondImageBaseName();
    const QString &file1DirPath = imagesInfo.getFirstImageDir();
    const QString &file2DirPath = imagesInfo.getSecondImageDir();
    QDir file1Dir { file1DirPath };
    QDir file2Dir { file2DirPath };
    QString fileName, fullPath;

    switch (saveImageInfo.saveImageInfoType) {
    case SaveImageInfoType::FirstImage:
        fileName = QString("%1%2").arg(file1Name, ext);
        fullPath = file1Dir.filePath(fileName);
        break;
    case SaveImageInfoType::SecondImage:
        fileName = QString("%1%2").arg(file2Name, ext);
        fullPath = file2Dir.filePath(fileName);
        break;
    case SaveImageInfoType::FirstImageArea:
        fileName = QString("%1_area%2").arg(file1Name, ext);
        fullPath = file1Dir.filePath(fileName);
        break;
    case SaveImageInfoType::SecondImageArea:
        fileName = QString("%1_area%2").arg(file2Name, ext);
        fullPath = file1Dir.filePath(fileName);
        break;
    case SaveImageInfoType::ComparisonImage:
        fileName = QString("%1_vs_%2_comparison%3")
                       .arg(file1Name, file2Name, ext);
        fullPath = file1Dir.filePath(fileName);
        break;
    case SaveImageInfoType::ComparisonImageArea:
        fileName = QString("%1_vs_%2_area_comparison%3")
                       .arg(file1Name, file2Name, ext);
        fullPath = file1Dir.filePath(fileName);
        break;
    default:
        break;
    }

    bool isSaved = false;
    SaveFileDialogHandler saveFileDialog {};
    auto savePath = saveFileDialog.getUserSaveImagePath(fullPath);
    if (savePath) {
        isSaved = saveImageInfo.image.save(savePath.value());
    }

    return std::make_optional<FileSaveResult>(isSaved, fullPath);
}
