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
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, this);
    show();
    mainWindow->onColorUnderCursorTrackingStatusChanged(true);
}

void ColorPickerController::placeColorPickerToRightSideOfMainWindow() {
    if (isVisible()) {
        mainWindow->addDockWidget(Qt::RightDockWidgetArea, this);
    }
}

void ColorPickerController::placeColorPickerToLeftSideOfMainWindow() {
    if (isVisible()) {
        mainWindow->addDockWidget(Qt::LeftDockWidgetArea, this);
    }
}

void ColorPickerController::onImagesClosed() {
    if (isVisible()) {
        colorPicker->reset();
    }
}

void ColorPickerController::onImagesOpened() {
    if (isVisible()) {
        mainWindow->onColorUnderCursorTrackingStatusChanged(true);
    }
}

void ColorPickerController::onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                                      const std::optional<ImagePixelColor> &hiddenImageRgbValue
                                                      )
{
    colorPicker->update(visibleImageRgbValue, hiddenImageRgbValue);
}
