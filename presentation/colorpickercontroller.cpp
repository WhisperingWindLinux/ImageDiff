#include "colorpickercontroller.h"
#include <QtGui/qscreen.h>
#include <qguiapplication.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include <presentation/mainwindow.h>
#include <presentation/dialogs/colorpickerpanel.h>

ColorPickerController::ColorPickerController(MainWindow *mainWindow)
    : mainWindow(mainWindow),
    alignmentPercent(80)
{
    colorPicker = std::make_unique<ColorPickerPanel>();
}

void ColorPickerController::onColorUnderCursorChanged(ImagePixelColor visibleImagePixelColor,
                                                      ImagePixelColor hiddenImagePixelColor
                                                      )
{
    colorPicker->update(visibleImagePixelColor, hiddenImagePixelColor);
}

void ColorPickerController::onImagesOpened() {
    if (colorPicker->isVisible()) {
        mainWindow->onColorUnderCursorTrackingStatusChanged(true);
    }
}

void ColorPickerController::onImagesClosed() {
    colorPicker->reset();
}

void ColorPickerController::onMainWindowClosed() {
    closeColorPickerDialog();
}

void ColorPickerController::onMainWindowStateChanged(bool isMinimized) {
    if (isMinimized) {
        colorPicker->hide();
    } else {
        colorPicker->show();
    }
}

void ColorPickerController::placeColorPickerToRightSideOfMainWindow() {
    if (colorPicker->isVisible()) {
        moveColorPickerPanel(PositionColorPickerWindow::Right);
    }
}

void ColorPickerController::placeColorPickerToLeftSideOfMainWindow() {
    if (colorPicker->isVisible()) {
        moveColorPickerPanel(PositionColorPickerWindow::Left);
    }
}

void ColorPickerController::openColorPickerDialog() {
    if (mainWindow->isMaximized() || mainWindow->isFullScreen()) {
        return;
    }
    colorPicker->show();
    mainWindow->onColorUnderCursorTrackingStatusChanged(true);
    placeColorPickerToRightSideOfMainWindow();
}

void ColorPickerController::closeColorPickerDialog() {
    colorPicker->close();
    mainWindow->onColorUnderCursorTrackingStatusChanged(false);
}

// Calculates the position of the color picker window relative to the main window (QMainWindow).
// Checks if the color picker window fits on the screen.
// If necessary, shifts the main window to the left or adjusts its size. The position of the second
// window vertically is set as a percentage (alignmentPercent) of the height of the main window.
// A value of 50% places the second window centered relative to the vertical axis of the main window.
void ColorPickerController::moveColorPickerPanel(PositionColorPickerWindow position)
{
    // Ensure alignmentPercent is within valid range
    alignmentPercent = qBound(0, alignmentPercent, 100);

    // Get screen geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry(); // Excludes taskbar/Dock area

    // Get main window geometry
    QRect mainGeometry = mainWindow->geometry();

    bool placeToRight = (position == PositionColorPickerWindow::Right);

    // Calculate target position for the second window
    int secondWindowX;
    if (placeToRight) {
        // Place the color picker to the right of the main window
        secondWindowX = mainGeometry.right() + 1;
    } else {
        // Place the color picker to the left of the main window
        secondWindowX = mainGeometry.left() - colorPicker->width() - 1;
    }

    int secondWindowY = mainGeometry.y() +
                        (mainGeometry.height() - colorPicker->height()) * (100 - alignmentPercent) / 100;

    // Adjust position if placing on the right and the second window goes off-screen horizontally
    if (placeToRight && secondWindowX + colorPicker->width() > screenGeometry.right()) {
        // Second window goes off-screen; adjust the main window
        int requiredShift = (secondWindowX + colorPicker->width()) - screenGeometry.right();
        if (mainGeometry.x() - requiredShift >= screenGeometry.left()) {
            // Move the main window to the left without resizing
            mainGeometry.moveLeft(mainGeometry.x() - requiredShift);
        } else {
            // Move the main window as far left as possible and resize it
            int maxShift = mainGeometry.x() - screenGeometry.left();
            mainGeometry.moveLeft(screenGeometry.left());
            mainGeometry.setWidth(mainGeometry.width() - (requiredShift - maxShift));
        }
        mainWindow->setGeometry(mainGeometry);
        // Recalculate the second window's position after adjusting the main window
        secondWindowX = mainGeometry.right() + 1;
    }

    // Adjust position if placing on the left and the second window goes off-screen horizontally
    if (!placeToRight && secondWindowX < screenGeometry.left()) {
        // Second window goes off-screen; adjust the main window
        int requiredShift = screenGeometry.left() - secondWindowX;
        if (mainGeometry.right() + requiredShift <= screenGeometry.right()) {
            // Move the main window to the right without resizing
            mainGeometry.moveLeft(mainGeometry.x() + requiredShift);
        } else {
            // Move the main window as far right as possible and resize it
            int maxShift = screenGeometry.right() - mainGeometry.right();
            mainGeometry.moveLeft(mainGeometry.x() + maxShift);
            mainGeometry.setWidth(mainGeometry.width() - (requiredShift - maxShift));
        }
        mainWindow->setGeometry(mainGeometry);
        // Recalculate the second window's position after adjusting the main window
        secondWindowX = mainGeometry.left() - colorPicker->width() - 1;
    }

    // Ensure the second window stays within vertical bounds of the screen
    if (secondWindowY < screenGeometry.top()) {
        secondWindowY = screenGeometry.top();
    } else if (secondWindowY + colorPicker->height() > screenGeometry.bottom()) {
        secondWindowY = screenGeometry.bottom() - colorPicker->height();
    }

    // Set the final position of the second window
    colorPicker->move(secondWindowX, secondWindowY);
}
