#ifndef VIDEOPLAYERWIDGET_H
#define VIDEOPLAYERWIDGET_H

#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QStyle>
#include <QDir>
#include <QImageWriter>
#include <QMediaPlayer>
#include <QVideoFrame>
#include <QVideoSink>
#include <qlabel.h>
#include <QMediaMetaData.h>

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
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QPushButton *playPauseButton; // Single button for Play/Pause
    QPushButton *screenshotButton;
    QSlider *slider;
    QLabel *frameLabel; // Displays the current frame number
    QLabel *timeLabel;  // Displays the current timestamp in milliseconds
    QString currentVideoPath;
    QString currentScreenshotPath;
    int screenshotCounter;
    double frameRate;
    qint64 currentPosition;
};

#endif // VIDEOPLAYERWIDGET_H
