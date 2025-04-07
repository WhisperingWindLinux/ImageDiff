#include "getimagesfromvideosinteractor.h"

#include <data/storage/filedialoghandler.h>

ImageHolderPtr GetImagesFromVideosInteractor::get() {
    // Load videos for both players
    FileDialogHandler service;
    auto pathsPair = service.getUserOpenTwoVideoPaths("");
    if (!pathsPair) {
        return nullptr; // the operation was canceled by the user
    }

    QString videoFilePath1 = pathsPair->first;
    QString videoFilePath2 = pathsPair->second;

    GetImagesFromVideosDialog dialog{nullptr, videoFilePath1, videoFilePath2};
    dialog.exec();
    if (dialog.isCanceled()) {
        return nullptr; // the operation was canceled by the user
    }
    QString firstImagePath = dialog.getFirstScreenshotPath();
    QString secondImagePath = dialog.getSecondScreenshotPath();

    return std::make_shared<ImageHolder>(QPixmap(),
                                         firstImagePath,
                                         QPixmap(),
                                         secondImagePath
                                         );
}
