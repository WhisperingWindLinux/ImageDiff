#ifndef COLORINFOPANEL_H
#define COLORINFOPANEL_H

#include "RgbValue.h"

#include <QObject>
#include <QWidget>
#include <qboxlayout.h>
#include <qframe.h>
#include <qlabel.h>

struct RgbWidgets {
    QLabel* fileNamelabel;     // The name of the file
    QVBoxLayout* panelLayout;
    QFrame* colorSquare;       // The square that shows the color (first panel)
    QLabel* rLabel;            // Label for R value (first panel)
    QLabel* gLabel;            // Label for G value (first panel)
    QLabel* bLabel;            // Label for B value (first panel)
};

class QLineEdit;

class ColorInfoPanel : public QWidget
{
    Q_OBJECT
public:
    // One panel displays the RGB values only for the visible image.
    // The second panel is for the second (hidden) image.
    explicit ColorInfoPanel(bool isOnePanelMode = true);

    void updateTopPanel(RgbValue rgbValue);
    void updateBottomPanel(RgbValue rgbValue);
    void updateBothPanelsAndHighlightDifferences(RgbValue topRgb,
                                                 RgbValue bottomRgb,
                                                 bool showDifferenceForSmallerComponents = true
                                                 );
private:
    RgbWidgets createPanel();  // Helper method to create a single panel

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

    bool isForVisibleImageOnly;
};


#endif // COLORINFOPANEL_H
