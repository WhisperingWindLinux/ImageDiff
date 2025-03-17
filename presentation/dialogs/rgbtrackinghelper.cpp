#include "rgbtrackinghelper.h"

#include <QtGui/qscreen.h>
#include <qguiapplication.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include <presentation/mainwindow.h>

RgbTrackingHelper::RgbTrackingHelper(MainWindow *mainWindow)
    : mainWindow(mainWindow)
{

}

void RgbTrackingHelper::onRgbValueUnderCursonChanged(RgbValue visibleImageRgbValue,
                                                         RgbValue hiddenImageRgbValue
                                                         )
{
    if (colorPanel == nullptr) {
        return;
    }
    if (colorPanel->isVisible()) {
        colorPanel->updateBothPanelsAndHighlightDifferences(visibleImageRgbValue, hiddenImageRgbValue);
    }
}

void RgbTrackingHelper::showColorPicker() {
    openColorPickerDialog(true);
}

void RgbTrackingHelper::showAdvancedColorPicker() {
    openColorPickerDialog(false);
}

void RgbTrackingHelper::onImageViewDestroyed() {
    closeColorPickerDialog();
}

void RgbTrackingHelper::onMainWindowClosed() {
    closeColorPickerDialog();
}

void RgbTrackingHelper::onMainWindowStateChanged(bool isMinimized) {
    if (isMinimized) {
        if (colorPanel != nullptr) { colorPanel->hide(); }
    } else {
        if (colorPanel != nullptr) { colorPanel->show(); }
    }
}

void RgbTrackingHelper::placeColorPickerOnRight() {
    if (colorPanel != nullptr && colorPanel->isVisible()) {
        positionColorPickerWindow(true);
    } else {
        openColorPickerDialog(false);
        positionColorPickerWindow(true);
    }
}

void RgbTrackingHelper::placeColorPickerOnLeft() {
    if (colorPanel != nullptr && colorPanel->isVisible()) {
        positionColorPickerWindow(false);
    } else {
        openColorPickerDialog(false);
        positionColorPickerWindow(true);
    }
}

void RgbTrackingHelper::openColorPickerDialog(bool isOnePanelMode) {
    if (mainWindow->isMaximized() || mainWindow->isFullScreen()) {
        return;
    }
    closeColorPickerDialog();
    colorPanel = new ColorInfoPanel(isOnePanelMode);
    colorPanel->show();
    mainWindow->onRgbTrackingStatusChanged(true);
    placeColorPickerOnRight();
}

std::shared_ptr<RgbTrackingState> RgbTrackingHelper::getCurrentState() {
    // TBD
    return make_shared<RgbTrackingState>();
}

void RgbTrackingHelper::setState(std::shared_ptr<RgbTrackingState> newState) {
    // TBD
}

void RgbTrackingHelper::closeColorPickerDialog() {
    if (colorPanel != nullptr) {
        colorPanel->close();
        delete colorPanel;
        colorPanel = nullptr;
    }
    mainWindow->onRgbTrackingStatusChanged(false);
}

// Calculates the position of the color picker window relative to the main window (QMainWindow).
// Checks if the color picker window fits on the screen.
// If necessary, shifts the main window to the left or adjusts its size. The position of the second
// window vertically is set as a percentage (alignmentPercent) of the height of the main window.
// A value of 50% places the second window centered relative to the vertical axis of the main window.
void RgbTrackingHelper::positionColorPickerWindow(bool placeOnRight)
{
    // Ensure alignmentPercent is within valid range
    alignmentPercent = qBound(0, alignmentPercent, 100);

    // Get screen geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry(); // Excludes taskbar/Dock area

    // Get main window geometry
    QRect mainGeometry = mainWindow->geometry();

    // Calculate target position for the second window
    int secondWindowX;
    if (placeOnRight) {
        // Place the color picker to the right of the main window
        secondWindowX = mainGeometry.right() + 1; // Start just to the right of the main window
    } else {
        // Place the color picker to the left of the main window
        secondWindowX = mainGeometry.left() - colorPanel->width() - 1; // Start just to the left of the main window
    }

    int secondWindowY = mainGeometry.y() +
                        (mainGeometry.height() - colorPanel->height()) * (100 - alignmentPercent) / 100;

    // Adjust position if placing on the right and the second window goes off-screen horizontally
    if (placeOnRight && secondWindowX + colorPanel->width() > screenGeometry.right()) {
        // Second window goes off-screen; adjust the main window
        int requiredShift = (secondWindowX + colorPanel->width()) - screenGeometry.right();
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
    if (!placeOnRight && secondWindowX < screenGeometry.left()) {
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
        secondWindowX = mainGeometry.left() - colorPanel->width() - 1;
    }

    // Ensure the second window stays within vertical bounds of the screen
    if (secondWindowY < screenGeometry.top()) {
        secondWindowY = screenGeometry.top();
    } else if (secondWindowY + colorPanel->height() > screenGeometry.bottom()) {
        secondWindowY = screenGeometry.bottom() - colorPanel->height();
    }

    // Set the final position of the second window
    colorPanel->move(secondWindowX, secondWindowY);
}
