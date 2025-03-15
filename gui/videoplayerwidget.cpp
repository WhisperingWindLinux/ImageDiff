#include "videoplayerwidget.h"

VideoPlayerWidget::VideoPlayerWidget(QWidget *parent)
    : QWidget(parent), screenshotCounter(0), frameRate(INFINITY), currentPosition(0) {
    // Set minimum size to prevent hidden widgets
    setMinimumSize(600, 600);

    // Main layout for the video player widget
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Video widget
    videoWidget = new QVideoWidget(this);
    layout->addWidget(videoWidget, 1); // Set stretch factor to 1 (takes all available space)

    // Control buttons (Play/Pause and Screenshot)
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    playPauseButton = new QPushButton("Play", this); // Single button for Play/Pause
    screenshotButton = new QPushButton("Take Screenshot", this);
    controlsLayout->addWidget(playPauseButton);
    controlsLayout->addWidget(screenshotButton);
    layout->addLayout(controlsLayout);

    // Slider for seeking through the video
    slider = new QSlider(Qt::Horizontal, this);
    layout->addWidget(slider);

    // Frame number and timestamp display
    QHBoxLayout *infoLayout = new QHBoxLayout();
    frameLabel = new QLabel("Frame: 0", this); // Displays the current frame number
    timeLabel = new QLabel("Time: 00:00:000", this); // Displays the current time in milliseconds
    infoLayout->addWidget(frameLabel);
    infoLayout->addWidget(timeLabel);
    layout->addLayout(infoLayout);

    // Add stretch at the end to ensure labels take minimal space
    layout->setStretchFactor(videoWidget, 1); // Video widget takes all available space
    layout->setStretchFactor(controlsLayout, 0); // Controls take minimal space
    layout->setStretchFactor(slider, 0);       // Slider takes minimal space
    layout->setStretchFactor(infoLayout, 0);   // Labels take minimal space

    // Media player setup
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setVideoOutput(videoWidget);

    // Connect signals and slots
    connect(playPauseButton, &QPushButton::clicked, this, &VideoPlayerWidget::togglePlayPause);
    connect(screenshotButton, &QPushButton::clicked, this, &VideoPlayerWidget::takeScreenshot);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayerWidget::updateSliderRange);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayerWidget::updateSliderPosition);
    connect(slider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);

    connect(mediaPlayer, &QMediaPlayer::metaDataChanged, [&]() {
        if (mediaPlayer->metaData().keys().contains(QMediaMetaData::VideoFrameRate)) {
            QVariant frameRate = mediaPlayer->metaData().value(QMediaMetaData::VideoFrameRate);
            this->frameRate = frameRate.toDouble();
        }
    });
}

void VideoPlayerWidget::loadVideo(const QString &filePath) {
    if (!filePath.isEmpty()) {
        mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
        currentVideoPath = filePath;
        mediaPlayer->play();
        mediaPlayer->pause();
        playPauseButton->setText("Pause"); // Update button to "Pause"
    }
}

int VideoPlayerWidget::getScreenshotCounter() const {
    return screenshotCounter;
}

QString VideoPlayerWidget::getCurrentScreenshotPath() const {
    return currentScreenshotPath;
}

void VideoPlayerWidget::togglePlayPause() {
    // Toggle between Play and Pause states
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
        playPauseButton->setText("Play");
    } else {
        mediaPlayer->play();
        playPauseButton->setText("Pause");
    }
}

void VideoPlayerWidget::takeScreenshot() {
    if (!mediaPlayer || !videoWidget) return;

    QVideoSink *videoSink = mediaPlayer->videoSink();
    if (!videoSink || !videoSink->videoFrame().isValid()) {
        throw std::runtime_error("No valid video frame available for screenshot.");
    }

    QVideoFrame frame = videoSink->videoFrame();
    frame.map(QVideoFrame::ReadOnly);
    QImage image = frame.toImage();
    frame.unmap();

    if (image.isNull()) {
        throw std::runtime_error("Failed to convert video frame to image.");
    }

    // Generate the file name
    QFileInfo fileInfo(currentVideoPath);
    QString baseName = fileInfo.completeBaseName(); // Video name without extension

    QString timePos = currentTimePositionAsString();

    QString screenshotFileName = QString("%1_screenshot_%2.png")
                                     .arg(baseName)
                                     .arg(timePos);

    // Save the screenshot in the same folder as the video
    currentScreenshotPath = fileInfo.absolutePath() + "/" + screenshotFileName;
    image.save(currentScreenshotPath);

    qDebug() << "Screenshot saved:" << currentScreenshotPath;

    screenshotButton->setDisabled(true);
    playPauseButton->setDisabled(true);
    slider->setDisabled(true);

    emit screenshotTaken();
}

void VideoPlayerWidget::updateSliderRange(qint64 duration) {
    slider->setRange(0, static_cast<int>(duration));
}

void VideoPlayerWidget::updateSliderPosition(qint64 position) {
    currentPosition = position;

    slider->setValue(static_cast<int>(position));

    // Update frame number and timestamp labels
    qint64 duration = mediaPlayer->duration();

    if (frameRate != INFINITY) {
        int frameNumber = static_cast<int>((position / static_cast<double>(duration)) * frameRate);
        frameLabel->setText(QString("Frame: %1").arg(frameNumber));
    } else {
        frameLabel->setText(QString("Frame: unknown"));
    }

    int ms = position % 1000;
    int seconds = (position / 1000) % 60;
    int minutes = (position / (1000 * 60)) % 60;
    timeLabel->setText(QString("Time: %1:%2:%3")
                           .arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0'))
                           .arg(ms, 3, 10, QChar('0')));
}

QString VideoPlayerWidget::currentTimePositionAsString() {
    int ms = currentPosition % 1000;
    int seconds = (currentPosition / 1000) % 60;
    int minutes = (currentPosition / (1000 * 60)) % 60;
    QString timePos = QString("%1_%2_%3")
                          .arg(minutes, 2, 10, QChar('0'))
                          .arg(seconds, 2, 10, QChar('0'))
                          .arg(ms, 3, 10, QChar('0'));
    return timePos;
}
