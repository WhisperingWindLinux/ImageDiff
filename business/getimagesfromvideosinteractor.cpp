#include "getimagesfromvideosinteractor.h"

#include <data/storage/savefiledialoghandler.h>

ImagesPtr GetImagesFromVideosInteractor::get() {
    // Load videos for both players
    SaveFileDialogHandler service;
    auto pathsPair = service.getUserOpenTwoVideoPaths("");
    if (!pathsPair) {
        return nullptr; // the operation was canceled by the user
    }

    QString videoFilePath1 = pathsPair->second;
    QString videoFilePath2 = pathsPair->second;

    GetImagesFromVideosDialog dialog {nullptr, videoFilePath1, videoFilePath2};
    dialog.exec();
    if (dialog.isCanceled()) {
        return nullptr; // the operation was canceled by the user
    }
    QString firstImagePath = dialog.getFirstScreenshotPath();
    QString secondImagePath = dialog.getSecondScreenshotPath();

    return std::make_shared<Images>(QPixmap(), QPixmap(), firstImagePath, secondImagePath);
}
