#include "getimagesfromvideosdialog.h"


GetImagesFromVideosDialog::GetImagesFromVideosDialog(QWidget *parent,
                                                       const QString &videoFilePath1,
                                                       const QString &videoFilePath2)
    : QDialog(parent),
    mVideoFilePath1(videoFilePath1),
    mVideoFilePath2(videoFilePath2),
    mTotalScreenshotsTaken(0)
{
    setWindowTitle("Get Images From Videos");

    // Main layout for the dialog
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Create two video player widgets
    mPlayer1 = new VideoPlayerWidget(this);
    mPlayer2 = new VideoPlayerWidget(this);

    mainLayout->addWidget(mPlayer1);
    mainLayout->addWidget(mPlayer2);

    if (!videoFilePath1.isEmpty() && !videoFilePath2.isEmpty()) {
        mPlayer1->loadVideo(videoFilePath1);
        mPlayer2->loadVideo(videoFilePath2);
    }

    // Connect screenshot signals from both players
    connect(mPlayer1, &VideoPlayerWidget::screenshotTaken, this, &GetImagesFromVideosDialog::handleScreenshotTaken);
    connect(mPlayer2, &VideoPlayerWidget::screenshotTaken, this, &GetImagesFromVideosDialog::handleScreenshotTaken);
}

QString GetImagesFromVideosDialog::getFirstScreenshotPath() {
    return mPlayer1->getCurrentScreenshotPath();
}

QString GetImagesFromVideosDialog::getSecondScreenshotPath() {
    return mPlayer2->getCurrentScreenshotPath();
}

bool GetImagesFromVideosDialog::isCanceled() {
    return mTotalScreenshotsTaken < 2;
}

void GetImagesFromVideosDialog::handleScreenshotTaken() {
    mTotalScreenshotsTaken++;
    if (mTotalScreenshotsTaken >= 2) {
        close(); // Close the dialog after two screenshots are taken
    }
}
