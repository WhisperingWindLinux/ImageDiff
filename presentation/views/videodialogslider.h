#ifndef VIDEODIALOGSLIDER_H
#define VIDEODIALOGSLIDER_H

#include <QSlider>
#include <qstyle.h>
#include <QtGui/qevent.h>

// Clicking on the slider's track area is supported by this class.
class VideoDialogSlider : public QSlider {

Q_OBJECT

Q_SIGNALS:
    void sliderClicked(int position);

public:
    VideoDialogSlider(Qt::Orientation orientation, QWidget *parent = nullptr);
    virtual ~VideoDialogSlider() = default;

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // VIDEODIALOGSLIDER_H
