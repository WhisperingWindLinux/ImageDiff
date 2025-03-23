#include "colorpickercontroller.h"

#include <presentation/dialogs/colorpickerpanel.h>
#include <presentation/mainwindow.h>

ColorPickerController::ColorPickerController(MainWindow *mainWindow)
    : QDockWidget(nullptr),
    mainWindow(mainWindow)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QObject::connect(this, &QDockWidget::dockLocationChanged, this, &ColorPickerController::onDockLocationChanged);
}

ColorPickerController::~ColorPickerController() {
    if (colorPicker != nullptr) {
        delete colorPicker;
        colorPicker = nullptr;
    }
}

void ColorPickerController::onDockLocationChanged(Qt::DockWidgetArea area) {
    if (!colorPicker) {
        return;
    }
    if (area == Qt::LeftDockWidgetArea) {
        colorPicker = new ColorPickerPanel(false);
        setWidget(colorPicker);
    } else {
        colorPicker = new ColorPickerPanel(true);
        setWidget(colorPicker);
    }
    if (area == Qt::NoDockWidgetArea) {
        adjustSize();
    } else {
        setMinimumWidth(150);
    }
}

void ColorPickerController::openColorPickerDialog() {
    if (!colorPicker) {
        return;
    }
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, this);
    show();
    mainWindow->onColorUnderCursorTrackingStatusChanged(true);
}

void ColorPickerController::placeColorPickerToRightSideOfMainWindow() {
    if (!colorPicker) {
        return;
    }
    if (isVisible()) {
        mainWindow->addDockWidget(Qt::RightDockWidgetArea, this);
    }
}

void ColorPickerController::placeColorPickerToLeftSideOfMainWindow() {
    if (!colorPicker) {
        return;
    }
    if (isVisible()) {
        mainWindow->addDockWidget(Qt::LeftDockWidgetArea, this);
    }
}

void ColorPickerController::onImagesClosed() {
    if (!colorPicker) {
        return;
    }
    colorPicker->reset();
}

void ColorPickerController::onImagesOpened() {
    if (!colorPicker) {
        return;
    }
    if (isVisible()) {
        mainWindow->onColorUnderCursorTrackingStatusChanged(true);
    }
}

void ColorPickerController::onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                                     const ImagePixelColor &hiddenImageRgbValue
                                                     )
{
    if (!colorPicker) {
        return;
    }
    colorPicker->update(visibleImageRgbValue, hiddenImageRgbValue);
}
