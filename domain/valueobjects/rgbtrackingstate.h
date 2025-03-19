#ifndef RGBTRACKINGSTATE_H
#define RGBTRACKINGSTATE_H

#include <QtCore/qpoint.h>

class RgbTrackingState {

public:
    RgbTrackingState(QPoint position, bool isOpenedInOnePanelMode)
        : position(std::move(position)),
        isOpenedInOnePanelMode(isOpenedInOnePanelMode)
    {}

    QPoint getPosition() const {
        return position;
    }

    bool getIsOpenedInOnePanelMode() const {
        return isOpenedInOnePanelMode;
    }

private:
    QPoint position;
    bool isOpenedInOnePanelMode;
};

#endif // RGBTRACKINGSTATE_H
