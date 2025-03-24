#include "getimagesfromvideosdialog.h"


GetImagesFromVideosDialog::GetImagesFromVideosDialog(QWidget *parent,
                                                       const QString &videoFilePath1,
                                                       const QString &videoFilePath2)
    : QDialog(parent),
    videoFilePath1(videoFilePath1),
    videoFilePath2(videoFilePath2),
    totalScreenshotsTaken(0)
{
    setWindowTitle("Get Images From Videos");

    // Main layout for the dialog
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Create two video player widgets
    player1 = new VideoPlayerWidget(this);
    player2 = new VideoPlayerWidget(this);

    mainLayout->addWidget(player1);
    mainLayout->addWidget(player2);

    if (!videoFilePath1.isEmpty() && !videoFilePath2.isEmpty()) {
        player1->loadVideo(videoFilePath1);
        player2->loadVideo(videoFilePath2);
    }

    // Connect screenshot signals from both players
    connect(player1, &VideoPlayerWidget::screenshotTaken, this, &GetImagesFromVideosDialog::handleScreenshotTaken);
    connect(player2, &VideoPlayerWidget::screenshotTaken, this, &GetImagesFromVideosDialog::handleScreenshotTaken);
}

QString GetImagesFromVideosDialog::getFirstScreenshotPath() {
    return player1->getCurrentScreenshotPath();
}

QString GetImagesFromVideosDialog::getSecondScreenshotPath() {
    return player2->getCurrentScreenshotPath();
}

bool GetImagesFromVideosDialog::isCanceled() {
    return totalScreenshotsTaken < 2;
}

void GetImagesFromVideosDialog::handleScreenshotTaken() {
    totalScreenshotsTaken++;
    if (totalScreenshotsTaken >= 2) {
        close(); // Close the dialog after two screenshots are taken
    }
}
