#include "videoplayerwidget.h"

#include <qmessagebox.h>
#include <presentation/views/videodialogslider.h>
#include <business/validation/imagevalidationrulesfactory.h>

VideoPlayerWidget::VideoPlayerWidget(QWidget *parent)
    : QWidget(parent), mScreenshotCounter(0), mFrameRate(INFINITY), mCurrentPosition(0) {
    setMinimumSize(600, 600);

    // Main layout for the video player widget
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Video widget
    mVideoWidget = new QVideoWidget(this);
    layout->addWidget(mVideoWidget, 1);

    // Control buttons (Play/Pause and Screenshot)
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    mPlayPauseButton = new QPushButton("Play", this); // Single button for Play/Pause
    mScreenshotButton = new QPushButton("Take Screenshot", this);
    controlsLayout->addWidget(mPlayPauseButton);
    controlsLayout->addWidget(mScreenshotButton);
    layout->addLayout(controlsLayout);

    // Slider for seeking through the video
    mSlider = new VideoDialogSlider(Qt::Horizontal, this);
    layout->addWidget(mSlider);

    // Frame number and timestamp display
    QHBoxLayout *infoLayout = new QHBoxLayout();
    mFrameLabel = new QLabel("Frame: 0", this); // Displays the current frame number
    mTimeLabel = new QLabel("Time: 00:00:000", this); // Displays the current time in milliseconds
    infoLayout->addWidget(mFrameLabel);
    infoLayout->addWidget(mTimeLabel);
    layout->addLayout(infoLayout);

    // Add stretch at the end to ensure labels take minimal space
    layout->setStretchFactor(mVideoWidget, 1); // Video widget takes all available space
    layout->setStretchFactor(controlsLayout, 0); // Controls take minimal space
    layout->setStretchFactor(mSlider, 0);       // Slider takes minimal space
    layout->setStretchFactor(infoLayout, 0);   // Labels take minimal space

    // Media player setup
    mMediaPlayer = new QMediaPlayer(this);
    mMediaPlayer->setVideoOutput(mVideoWidget);

    connect(mPlayPauseButton, &QPushButton::clicked, this, &VideoPlayerWidget::togglePlayPause);
    connect(mScreenshotButton, &QPushButton::clicked, this, &VideoPlayerWidget::takeScreenshot);
    connect(mMediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayerWidget::updateSliderRange);
    connect(mMediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayerWidget::updateSliderPosition);
    connect(mSlider, &QSlider::sliderMoved, mMediaPlayer, &QMediaPlayer::setPosition);
    connect(mSlider, &VideoDialogSlider::sliderClicked, mMediaPlayer, &QMediaPlayer::setPosition);

    connect(mMediaPlayer, &QMediaPlayer::metaDataChanged, this, [&]() {
        if (mMediaPlayer->metaData().keys().contains(QMediaMetaData::VideoFrameRate)) {
            QVariant frameRate = mMediaPlayer->metaData().value(QMediaMetaData::VideoFrameRate);
            this->mFrameRate = frameRate.toDouble();
        }
    });
}

void VideoPlayerWidget::loadVideo(const QString &filePath) {
    if (!filePath.isEmpty()) {
        mMediaPlayer->setSource(QUrl::fromLocalFile(filePath));
        mCurrentVideoPath = filePath;
        mMediaPlayer->play();
        mMediaPlayer->pause();
        mPlayPauseButton->setText("Play");
    }
}

int VideoPlayerWidget::getScreenshotCounter() const {
    return mScreenshotCounter;
}

QString VideoPlayerWidget::getCurrentScreenshotPath() const {
    return mCurrentScreenshotPath;
}

void VideoPlayerWidget::togglePlayPause() {
    // Toggle between Play and Pause states
    if (mMediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mMediaPlayer->pause();
        mPlayPauseButton->setText("Play");
    } else {
        mMediaPlayer->play();
        mPlayPauseButton->setText("Pause");
    }
}

void VideoPlayerWidget::takeScreenshot() {
    if (!mMediaPlayer || !mVideoWidget) return;

    QVideoSink *videoSink = mMediaPlayer->videoSink();
    if (!videoSink || !videoSink->videoFrame().isValid()) {
        QMessageBox::critical(this, "Error", "No valid video frame available for screenshot.");
        return;
    }

    QVideoFrame frame = videoSink->videoFrame();
    frame.map(QVideoFrame::ReadOnly);
    QImage image = frame.toImage();
    frame.unmap();

    if (image.isNull()) {
        QMessageBox::critical(this, "Error", "Failed to convert video frame to image!");
        return;
    }

    // Generate the file name
    QFileInfo fileInfo(mCurrentVideoPath);
    QString baseName = fileInfo.completeBaseName(); // Video name without extension

    QString timePos = currentTimePositionAsString();

    auto extentionValidator = ImageValidationRulesFactory::createImageExtensionsInfoProvider();
    QString ext = extentionValidator->getDeafaultSaveExtension(true);

    QString screenshotFileName = QString("%1_screenshot_%2%3")
                                     .arg(baseName, timePos, ext);

    // Save the screenshot in the same folder as the video
    mCurrentScreenshotPath = fileInfo.absolutePath() + "/" + screenshotFileName;
    image.save(mCurrentScreenshotPath);

    qDebug() << "Screenshot saved:" << mCurrentScreenshotPath;

    mScreenshotButton->setDisabled(true);
    mPlayPauseButton->setDisabled(true);
    mSlider->setDisabled(true);

    emit screenshotTaken();
}

void VideoPlayerWidget::updateSliderRange(qint64 duration) {
    mSlider->setRange(0, static_cast<int>(duration));
}

void VideoPlayerWidget::updateSliderPosition(qint64 position) {
    mCurrentPosition = position;

    mSlider->setValue(static_cast<int>(position));

    // Update frame number and timestamp labels
    qint64 duration = mMediaPlayer->duration();

    if (mFrameRate != INFINITY) {
        int frameNumber = static_cast<int>((position / static_cast<double>(duration)) * mFrameRate);
        mFrameLabel->setText(QString("Frame: %1").arg(frameNumber));
    } else {
        mFrameLabel->setText(QString("Frame: unknown"));
    }

    int ms = position % 1000;
    int seconds = (position / 1000) % 60;
    int minutes = (position / (1000 * 60)) % 60;
    mTimeLabel->setText(QString("Time: %1:%2:%3")
                           .arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0'))
                           .arg(ms, 3, 10, QChar('0')));
}

QString VideoPlayerWidget::currentTimePositionAsString() {
    int ms = mCurrentPosition % 1000;
    int seconds = (mCurrentPosition / 1000) % 60;
    int minutes = (mCurrentPosition / (1000 * 60)) % 60;
    QString timePos = QString("%1_%2_%3")
                          .arg(minutes, 2, 10, QChar('0'))
                          .arg(seconds, 2, 10, QChar('0'))
                          .arg(ms, 3, 10, QChar('0'));
    return timePos;
}
