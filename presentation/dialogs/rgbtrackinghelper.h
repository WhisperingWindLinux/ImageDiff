#ifndef RGBTRACKINGHELPER_H
#define RGBTRACKINGHELPER_H

#include <domain/valueobjects/rgbtrackingstate.h>
#include <domain/valueobjects/rgbvalue.h>
#include <presentation/dialogs/colorinfopanel.h>

using namespace std;

class MainWindow;
class ColorInfoPanel;

class RgbTrackingHelper
{
public:
    RgbTrackingHelper(MainWindow *mainWindow);

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

    optional<RgbTrackingState> getCurrentState();
    void setState(optional<RgbTrackingState> newState);

private:
    ColorInfoPanel* colorPanel = nullptr;
    MainWindow *mainWindow = nullptr;

    // A value of 50% places the color picker window centered
    // relative to the vertical axis of the main window.
    int alignmentPercent = 80;
};

#endif // RGBTRACKINGHELPER_H
