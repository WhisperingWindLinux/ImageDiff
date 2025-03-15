#ifndef RGBTRACKINGINTERACTOR_H
#define RGBTRACKINGINTERACTOR_H

#include <gui/RgbValue.h>
#include <gui/colorinfopanel.h>

class MainWindow;
class QWidget;
class ColorInfoPanel;
class QCloseEvent;
class QEvent;

class RgbTrackingInteractor
{
public:
    RgbTrackingInteractor(MainWindow *mainWindow);

    void positionColorPickerWindow(bool placeOnRight);
    void onMainWindowStateChanged(bool isMinimized);
    void placeColorPickerOnRight();
    void placeColorPickerOnLeft();
    void closeColorPickerDialog();
    void onMainWindowClosed();
    void onImageViewDestroyed();
    void showColorPicker();
    void showAdvancedColorPicker();
    void onRgbValueUnderCursonChanged(RgbValue visibleImageRgbValue, RgbValue hiddenImageRgbValue);
    void openColorPickerDialog(bool isOnePanelMode);

private:
    ColorInfoPanel* colorPanel = nullptr;
    MainWindow *mainWindow = nullptr;

    // A value of 50% places the color picker window centered
    // relative to the vertical axis of the main window.
    int alignmentPercent = 80;
};

#endif // RGBTRACKINGINTERACTOR_H
