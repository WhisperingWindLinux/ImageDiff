#ifndef COLORINFOPANEL_H
#define COLORINFOPANEL_H

#include <QObject>
#include <QWidget>
#include <domain/valueobjects/imagepixelcolor.h>
#include <presentation/valueobjects/rgbwidgets.h>

class QLineEdit;
class QLabel;
class QFrame;

// If RGB tracking is active, the dialog displays the RGB values of the pixel
// under the mouse cursor for both compared images. For this purpose,
// the dialog has two panels with RGB values.

class ColorPickerPanel : public QWidget
{
    Q_OBJECT
public:
    // One panel displays the RGB values only for the visible image.
    // The second panel is for the second (hidden) image.
    explicit ColorPickerPanel(bool isForRightPosition, QWidget *parent = nullptr, bool isTwoPanelMode = true);
    
    void update(const ImagePixelColor &visibleImageColor, std::optional<ImagePixelColor> hiddenImageColor);

    void reset();

private:
    RgbWidgets createPanel(bool isForRightPosition);  // Helper method to create a single panel

private:
    QLabel* firstFileNameLabel;
    QFrame* firstColorSquare;       // The square that shows the color (first panel)
    QLabel* firstRLabel;            // Label for R value (first panel)
    QLabel* firstBLabel;            // Label for G value (first panel)
    QLabel* firstGLabel;            // Label for B value (first panel)

    QLabel* secondFileNameLabel = nullptr;
    QFrame* secondColorSquare = nullptr; // The square that shows the color (second panel)
    QLabel* secondRLabel = nullptr;      // Label for R value (second panel)
    QLabel* secondGLabel = nullptr;      // Label for G value (second panel)
    QLabel* secondBLabel = nullptr;      // Label for B value (second panel)

    bool isTwoPanelMode;

    QLayout *panelMainLayout;
    
    void updateTopPanelOnly(const ImagePixelColor &firstPanelValue);
    void setLayout(bool isForRightPosition);

    QString format(const QString &colorComponemt,
                   int color,
                   int diff,
                   bool alignColorValueLeft = true,
                   bool alignDiffValueLeft = true
                   );
};


#endif // COLORINFOPANEL_H
