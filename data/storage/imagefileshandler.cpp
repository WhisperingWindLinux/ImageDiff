#include "imagefileshandler.h"

#include <QStringList>
#include <business/recentfilesmanager.h>
#include <presentation/presenters/recentfilespresenter.h>
#include <business/imageanalysis/comparisoninteractor.h>
#include <data/storage/savefiledialoghandler.h>

ImageFilesHandler::ImageFilesHandler(IMainWindowCallbacks *callbacks)
    : callbacks(callbacks)
{
    imageRepository = std::make_unique<ImagesRepository>();
}

void ImageFilesHandler::openImagesFromDragAndDrop(QList<QUrl> urls) {
    if (urls.size() != 2) {
        return;
    }
    if (urls[0].isLocalFile() && urls[1].isLocalFile()) {
        QString file1 = urls[0].toLocalFile();
        QString file2 = urls[1].toLocalFile();
        openImages(file1, file2, false, true);
    }
}

void ImageFilesHandler::getPathsFromUserAndOpenImages() {
    SaveFileDialogHandler service;
    OptionalPathPair twoUserImagePaths = service.getUserOpenTwoImagePaths("");
    if (!twoUserImagePaths) {
        return;
    }
    openImages(twoUserImagePaths.value().first,
               twoUserImagePaths.value().second,
               false,
               true
               );
}


void ImageFilesHandler::openImages(QString& Image1Path,
                                      QString& Image2Path,
                                      bool removeImageFilesAtExit,
                                      bool isUpdateRecentMenu
                                      )
{
    bool isUpdated = imageRepository->update(Image1Path, Image2Path);
    cleanUpImageFilesAtExit = removeImageFilesAtExit;


    if (isUpdated) {
        QString errorMsg = QString("Error: Unable to load images; the files ") +
                           "are missing, or the application does not have access to them!";
        throw std::runtime_error(errorMsg.toStdString());
    }

    auto firstImage = imageRepository->getFirstImage();
    auto secondImage = imageRepository->getSecondImage();

    if (firstImage->size() != firstImage->size()) {
        throw std::runtime_error("Error: Images must have the same resolution!");
    }

    callbacks->displayImages(firstImage,
                             imageRepository->getFirstImagePath(),
                             secondImage,
                             imageRepository->getSecondImagePath()
                             );

    if (isUpdateRecentMenu) {
        recentFilesManager->addPair(firstImagePath, secondImagePath);
        callbacks->updateRecentFilesMenu();
    }

}



void ImageFilesHandler::saveImage(SaveImageInfo info) {
    if (info.saveImageInfoType == SaveImageInfoType::None ||
        info.image.isNull())
    {
        return;
    }

    QString fileName, fullPath;
    SaveFileDialogHandler fsAccess;

    switch (info.saveImageInfoType) {
    case SaveImageInfoType::FirstImage:
        callbacks->saveImage(info.image, imageRepositiory.);
        break;
    case SaveImageInfoType::SecondImage:
        callbacks->saveImage(info.image, secondImagePath);
        break;
    case SaveImageInfoType::FirstImageArea:
        fileName = file1Name + "_area" + defaultExtention;
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImage(info.image, fullPath);
        break;
    case SaveImageInfoType::SecondImageArea:
        fileName = file2Name + "_area" + defaultExtention;
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImage(info.image, fullPath);
        break;
    case SaveImageInfoType::ComparisonImage:
        fileName = QString("%1_vs_%2_comparison%3")
                       .arg(file1Name, file2Name, defaultExtention);
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImage(info.image, fullPath);
        break;
    case SaveImageInfoType::ComparisonImageArea:
        fileName = QString("%1_vs_%2_area_comparison%3")
                       .arg(file1Name, file2Name, defaultExtention);
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImage(info.image, fullPath);
        break;
    default:
        break;
    }
}
