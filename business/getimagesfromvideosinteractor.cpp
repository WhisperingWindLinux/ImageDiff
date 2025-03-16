#include "getimagesfromvideosinteractor.h"

GetImagesFromVideosInteractor::GetImagesFromVideosInteractor() {}

std::optional<QPair<QString, QString> > GetImagesFromVideosInteractor::grabImagesFromVideos() {
    // Load videos for both players
    QString filter = "Video Files (*.mp4 *.avi *.mkv *.mov *.wmv *.flv *.webm);;All Files (*)";
    QString videoFilePath1 = QFileDialog::getOpenFileName(nullptr, "Select a first video file", "", filter);
    QString videoFilePath2 = QFileDialog::getOpenFileName(nullptr, "Select a second video file", "", filter);

     if (videoFilePath1.isEmpty() || videoFilePath2.isEmpty()) {
        return std::nullopt;
     }

    QFileInfo videoFile1 {videoFilePath1};
    QFileInfo videoFile2 {videoFilePath2};

    if (!videoFile1.isFile() ||
            !videoFile2.isFile() ||
            !videoFile1.exists() ||
            !videoFile2.exists())
    {
        throw std::runtime_error("Error: Unable to open video files.");
    }

    GrabImagesFromVideosDialog dialog {nullptr, videoFilePath1, videoFilePath2};
    dialog.exec();
    if (dialog.isCanceled()) {
        return std::nullopt;
    }
    QString firstImagePath = dialog.getFirstScreenshotPath();
    QString secondImagePath = dialog.getSecondScreenshotPath();

    return { { firstImagePath, secondImagePath } };
}
