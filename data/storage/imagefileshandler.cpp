#include "imagefileshandler.h"

#include <QStringList>
#include <QtCore/qdir.h>
#include <qfileinfo.h>
#include <business/recentfilesmanager.h>
#include <business/imageanalysis/imageprocessinginteractor.h>
#include <data/storage/savefiledialoghandler.h>
#include <data/repositories/imagesrepository.h>
#include <domain/valueobjects/images.h>
#include <business/utils/imagesinfo.h>


const QString ImageFilesHandler::imageExtentionWithoutDot = "png";
const QString ImageFilesHandler::imageExtentionWithDot = QString(".") + imageExtentionWithoutDot;
const std::string ImageFilesHandler::errorUnableToOpenImages = "Error: Unable to load images!";

ImagesPtr ImageFilesHandler::openImages(const QList<QUrl> &urls) {
    if (urls.size() != 2) {
        throw std::runtime_error(errorUnableToOpenImages);
    }
    if (urls[0].isLocalFile() && urls[1].isLocalFile()) {
        const QString file1 = urls[0].toLocalFile();
        const QString file2 = urls[1].toLocalFile();
        return openImages(file1, file2);
    }
    throw std::runtime_error(errorUnableToOpenImages);
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

ImagesPtr ImageFilesHandler::openImages(const QString &image1Path, const QString &image2Path) {
    QFileInfo firstImageInfo { image1Path };
    QFileInfo secondImageInfo { image2Path };

    if (!firstImageInfo.isFile() || !secondImageInfo.isFile()) {
        throw std::runtime_error("Error: Unable to load images; "
                                 "one or both files do not exist!"
                                 );
    }

    QString suffix1 = "." + firstImageInfo.suffix().toLower();
    QString suffix2 = "." + secondImageInfo.suffix().toLower();
    if (suffix1 != suffix2 || suffix1 != imageExtentionWithDot) {
        throw std::runtime_error("Error: Unable to load images; "
                                 "one or both images have an unsupported format!"
                                 );
    }

    QImage image1, image2;
    image1.load(image1Path);
    image2.load(image2Path);

    if (image1.isNull() || image2.isNull()) {
        throw std::runtime_error(errorUnableToOpenImages);
    }

    if (image1.size() != image2.size()) {
        throw std::runtime_error("Error: Unable to load images; Images must have the same resolution!");
    }
    return std::make_shared<Images>(image1, image2, image1Path, image2Path);
}


FileSaveResult ImageFilesHandler::saveImage(const SaveImageInfo &saveImageInfo,
                                            const ImagesPtr images
                                           )
{
    if (saveImageInfo.saveImageInfoType == SaveImageInfoType::None ||
        saveImageInfo.image.isNull())
    {
        return { false, "unknown" };
    }

    ImagesInfo imagesInfo { images };

    const QString &file1Name = imagesInfo.getFirstImageBaseName();
    const QString &file2Name = imagesInfo.getSecondImageBaseName();
    const QString &path1 = imagesInfo.getFirstImagePath();
    const QString &path2 = imagesInfo.getSecondImagePath();
    const QString &file1DirPath = imagesInfo.getFirstImageDir();
    const QString &file2DirPath = imagesInfo.getSecondImageDir();
    QDir file1Dir { file1DirPath };
    QDir file2Dir { file2DirPath };
    QString fileName, fullPath;

    bool isSaved = false;

    switch (saveImageInfo.saveImageInfoType) {
    case SaveImageInfoType::FirstImage:
        fullPath = path1;
        isSaved = saveImageInfo.image.save(fullPath);
    case SaveImageInfoType::SecondImage:
        fullPath = path2;
        isSaved = saveImageInfo.image.save(fullPath);
    case SaveImageInfoType::FirstImageArea:
        fileName = file1Name + "_area" + imageExtentionWithDot;
        fullPath = file1Dir.filePath(fileName);
        isSaved = saveImageInfo.image.save(fullPath);
    case SaveImageInfoType::SecondImageArea:
        fileName = file2Name + "_area" + imageExtentionWithDot;
        fullPath = file1Dir.filePath(fileName);
        isSaved = saveImageInfo.image.save(fullPath);
    case SaveImageInfoType::ComparisonImage:
        fileName = QString("%1_vs_%2_comparison%3")
                       .arg(file1Name, file2Name, imageExtentionWithDot);
        fullPath = file1Dir.filePath(fileName);
        isSaved = saveImageInfo.image.save(fullPath);
    case SaveImageInfoType::ComparisonImageArea:
        fileName = QString("%1_vs_%2_area_comparison%3")
                       .arg(file1Name, file2Name, imageExtentionWithDot);
        fullPath = file1Dir.filePath(fileName);
        isSaved = saveImageInfo.image.save(fullPath);
    default:
        break;
    }

    return { isSaved, fullPath };
}
