#ifndef COLORPICKERCONTROLLER_H
#define COLORPICKERCONTROLLER_H

#include <domain/valueobjects/imagepixelcolor.h>

class MainWindow;
class ColorPickerPanel;

// Methods related to working with ColorPickerPanel

class ColorPickerController
{
    enum class PositionColorPickerWindow { Left, Right };

public:
    ColorPickerController(MainWindow *mainWindow);

    void openColorPickerDialog();
    void closeColorPickerDialog();
    void placeColorPickerToRightSideOfMainWindow();
    void placeColorPickerToLeftSideOfMainWindow();

    void onMainWindowClosed();
    void onImagesClosed();
    void onImagesOpened();
    void onColorUnderCursorChanged(ImagePixelColor visibleImageRgbValue,
                                   ImagePixelColor hiddenImageRgbValue
                                   );
    void onMainWindowStateChanged(bool isMinimized);

private:
    MainWindow *mainWindow;
    std::unique_ptr<ColorPickerPanel> colorPicker;

    // A value of 50% places the color picker window centered
    // relative to the vertical axis of the main window
    int alignmentPercent;

    // Moves the panel to the right or left side of the application window
    void moveColorPickerPanel(PositionColorPickerWindow position);
};

#endif // COLORPICKERCONTROLLER_H
