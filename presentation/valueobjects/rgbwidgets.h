#ifndef RGBWIDGETS_H
#define RGBWIDGETS_H

#include <qboxlayout.h>
#include <qlabel.h>

struct RgbWidgets {
    QLabel* fileNamelabel;     // The name of the file
    QVBoxLayout* panelLayout;
    QFrame* colorSquare;       // The square that shows the color (first panel)
    QLabel* rLabel;            // Label for R value (first panel)
    QLabel* gLabel;            // Label for G value (first panel)
    QLabel* bLabel;            // Label for B value (first panel)
};

#endif // RGBWIDGETS_H
