#include "videodialogslider.h"

VideoDialogSlider::VideoDialogSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{
}

void VideoDialogSlider::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        int newValue = QStyle::sliderValueFromPosition(
            minimum(), maximum(), event->pos().x(), width());

        emit sliderClicked(newValue);
    }
    QSlider::mousePressEvent(event);
}
