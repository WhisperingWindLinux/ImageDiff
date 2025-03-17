#include "getimagesfromvideosinteractor.h"
#include <data/getfileuserpathsservcie.h>

GetImagesFromVideosInteractor::GetImagesFromVideosInteractor() {}

std::optional<QPair<QString, QString> > GetImagesFromVideosInteractor::grabImagesFromVideos() {
    // Load videos for both players
    GetFileUserPathsService service;
    auto pathsPair = service.getUserOpenTwoVideoPaths("");
    if (!pathsPair) {
        return nullopt;
    }

    auto videoFilePath1 = pathsPair->second;
    auto videoFilePath2 = pathsPair->second;

    GrabImagesFromVideosDialog dialog {nullptr, videoFilePath1, videoFilePath2};
    dialog.exec();
    if (dialog.isCanceled()) {
        return nullopt;
    }
    QString firstImagePath = dialog.getFirstScreenshotPath();
    QString secondImagePath = dialog.getSecondScreenshotPath();

    return { { firstImagePath, secondImagePath } };
}
