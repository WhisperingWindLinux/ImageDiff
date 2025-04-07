#include "colorpickercontroller.h"

#include <presentation/dialogs/colorpickerpanel.h>
#include <presentation/mainwindow.h>

ColorPickerController::ColorPickerController(MainWindow *mainWindow)
    : QDockWidget(nullptr),
    mMainWindow(mainWindow)
{    
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QObject::connect(this, &QDockWidget::dockLocationChanged, this, &ColorPickerController::onDockLocationChanged);
}

ColorPickerController::~ColorPickerController() {
    if (mColorPicker != nullptr) {
        delete mColorPicker;
        mColorPicker = nullptr;
    }
}

void ColorPickerController::onDockLocationChanged(Qt::DockWidgetArea area) {
    if (area == Qt::LeftDockWidgetArea) {
        mColorPicker = new ColorPickerPanel(false);
        setWidget(mColorPicker);
    } else {
        mColorPicker = new ColorPickerPanel(true);
        setWidget(mColorPicker);
    }
    if (area == Qt::NoDockWidgetArea) {
        adjustSize();
    } else {
        setMinimumWidth(150);
    }
}

void ColorPickerController::openColorPickerDialog() {
    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, this);
    show();
    mMainWindow->onColorUnderCursorTrackingStatusChanged(true);
}

void ColorPickerController::placeColorPickerToRightSideOfMainWindow() {
    if (isVisible()) {
        mMainWindow->addDockWidget(Qt::RightDockWidgetArea, this);
    }
}

void ColorPickerController::placeColorPickerToLeftSideOfMainWindow() {
    if (isVisible()) {
        mMainWindow->addDockWidget(Qt::LeftDockWidgetArea, this);
    }
}

void ColorPickerController::onImagesClosed() {
    if (isVisible()) {
        mColorPicker->reset();
    }
}

void ColorPickerController::onImagesOpened() {
    if (isVisible()) {
        mMainWindow->onColorUnderCursorTrackingStatusChanged(true);
    }
}

void ColorPickerController::onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                                      const std::optional<ImagePixelColor> &hiddenImageRgbValue
                                                      )
{
    mColorPicker->update(visibleImageRgbValue, hiddenImageRgbValue);
}
