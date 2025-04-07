#ifndef VIDEOPLAYERWIDGET_H
#define VIDEOPLAYERWIDGET_H

#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QStyle>
#include <QDir>
#include <QImageWriter>
#include <QMediaPlayer>
#include <QVideoFrame>
#include <QVideoSink>
#include <qlabel.h>
#include <qmediametadata.h>

class VideoDialogSlider;

class VideoPlayerWidget : public QWidget {
    Q_OBJECT

public:
    VideoPlayerWidget(QWidget *parent = nullptr);

    void loadVideo(const QString &filePath);

    int getScreenshotCounter() const;

    QString getCurrentScreenshotPath() const;

private slots:
    void togglePlayPause();

    void takeScreenshot();

    void updateSliderRange(qint64 duration);

    void updateSliderPosition(qint64 position);

    QString currentTimePositionAsString();

signals:
    void screenshotTaken();

private:
    QMediaPlayer *mMediaPlayer;
    QVideoWidget *mVideoWidget;
    QPushButton *mPlayPauseButton; // Single button for Play/Pause
    QPushButton *mScreenshotButton;
    VideoDialogSlider *mSlider;
    QLabel *mFrameLabel; // Displays the current frame number
    QLabel *mTimeLabel;  // Displays the current timestamp in milliseconds
    QString mCurrentVideoPath;
    QString mCurrentScreenshotPath;
    int mScreenshotCounter;
    double mFrameRate;
    qint64 mCurrentPosition;
};

#endif // VIDEOPLAYERWIDGET_H
