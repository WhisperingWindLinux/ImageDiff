#include "getimagesfromvideosinteractor.h"

OptionalStringPair GetImagesFromVideosInteractor::getImagePaths() {
    // Load videos for both players
    FileDialogHandler service;
    auto pathsPair = service.getUserOpenTwoVideoPaths("");
    if (!pathsPair) {
        return std::nullopt; // the operation was canceled by the user
    }

    QString videoFilePath1 = pathsPair->first;
    QString videoFilePath2 = pathsPair->second;

    GetImagesFromVideosDialog dialog{nullptr, videoFilePath1, videoFilePath2};
    dialog.exec();
    if (dialog.isCanceled()) {
        return std::nullopt; // the operation was canceled by the user
    }
    auto firstImagePath = dialog.getFirstScreenshotPath();
    auto secondImagePath = dialog.getSecondScreenshotPath();

    if (!firstImagePath || !secondImagePath) {
        dialog.showError("Failed to save one or both screenshots.");
        return std::nullopt;
    }
    return std::make_optional<QPair<QString, QString> >(firstImagePath.value(),
                                                        secondImagePath.value()
                                                        );
}
