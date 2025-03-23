#include "imagefileshandler.h"

#include <QStringList>
#include <QtCore/qdir.h>
#include <qfileinfo.h>
#include <business/recentfilesmanager.h>
#include <business/imageanalysis/imageprocessinginteractor.h>
#include <data/storage/savefiledialoghandler.h>
#include <domain/valueobjects/images.h>
#include <business/utils/imagesinfo.h>
#include <business/imageanalysis/validation/imagevalidationrulesfactory.h>


ImagesPtr ImageFilesHandler::openImages(const QList<QUrl> &urls) {
    if (urls.size() != 2) {
        QString err = QString("Invalid number of images. ")
                      + "Drag and drop two images in PNG format.";
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
    QFileInfo firstImageInfo { image1Path };
    QFileInfo secondImageInfo { image2Path };

    QString error = "%1 file does not exist or this is not a PNG image. Please select an image in PNG format.";

    if (!firstImageInfo.isFile()) {
        throw std::runtime_error(error.arg(image1Path).toStdString());
    }
    if (!secondImageInfo.isFile()) {
        throw std::runtime_error(error.arg(image2Path).toStdString());
    }
    if (firstImageInfo.suffix() != "png" ||
        secondImageInfo.suffix() != "png") {
        throw std::runtime_error(error.arg(image1Path).toStdString());
    }

    QPixmap image1, image2;
    image1.load(image1Path);
    image2.load(image2Path);

    auto images = std::make_shared<Images>(image1, image2, image1Path, image2Path);

    validateImages(images);

    return images;
}

void ImageFilesHandler::validateImages(ImagesPtr images) {
    auto validationRules = ImageValidationRulesFactory::create(images);
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

    const QString &file1Name = imagesInfo.getFirstImageBaseName();
    const QString &file2Name = imagesInfo.getSecondImageBaseName();
    const QString &path1 = imagesInfo.getFirstImagePath();
    const QString &path2 = imagesInfo.getSecondImagePath();
    const QString &file1DirPath = imagesInfo.getFirstImageDir();
    QDir file1Dir { file1DirPath };
    QString fileName, fullPath;

    switch (saveImageInfo.saveImageInfoType) {
    case SaveImageInfoType::FirstImage:
        fullPath = path1;
        break;
    case SaveImageInfoType::SecondImage:
        fullPath = path2;
        break;
    case SaveImageInfoType::FirstImageArea:
        fileName = file1Name + "_area.png";
        fullPath = file1Dir.filePath(fileName);
        break;
    case SaveImageInfoType::SecondImageArea:
        fileName = file2Name + "_area.png";
        fullPath = file1Dir.filePath(fileName);
        break;
    case SaveImageInfoType::ComparisonImage:
        fileName = QString("%1_vs_%2_comparison.png")
                       .arg(file1Name, file2Name);
        fullPath = file1Dir.filePath(fileName);
        break;
    case SaveImageInfoType::ComparisonImageArea:
        fileName = QString("%1_vs_%2_area_comparison.png")
                       .arg(file1Name, file2Name);
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
