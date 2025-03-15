#include "colorinfopanel.h"
#include "gui/RgbValue.h"

#include <qboxlayout.h>
#include <qlineedit.h>

ColorInfoPanel::ColorInfoPanel(bool isForVisibleImageOnly)
    : QWidget{nullptr},
    isForVisibleImageOnly(isForVisibleImageOnly)
{
    setWindowTitle("Color Picker");

    // Create the main layout for the panel (vertical layout)
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10); // Add padding
    mainLayout->setSpacing(10); // Add spacing between elements

    // Create the first panel (visible image panel)
    auto rgbWidgets = createPanel();
    mainLayout->addLayout(rgbWidgets.panelLayout);
    firstFileNameLabel = rgbWidgets.fileNamelabel;
    firstColorSquare = rgbWidgets.colorSquare;
    firstRLabel = rgbWidgets.rLabel;
    firstGLabel = rgbWidgets.gLabel;
    firstBLabel = rgbWidgets.bLabel;

    // If isForVisibleImageOnly is false, create a advanced color picker' panel
    if (!isForVisibleImageOnly) {
        // Add some spacing before the second panel
        mainLayout->addSpacing(20);

        // Create the second panel
        auto rgbWidgets = createPanel();

        mainLayout->addLayout(rgbWidgets.panelLayout);

        // Store references to the second panel's elements
        secondFileNameLabel = rgbWidgets.fileNamelabel;
        secondColorSquare = rgbWidgets.colorSquare;
        secondRLabel = rgbWidgets.rLabel;
        secondGLabel = rgbWidgets.gLabel;
        secondBLabel = rgbWidgets.bLabel;
    }

    // Add a spacer to push everything to the top of the panel
    mainLayout->addStretch();

    setWindowFlags(Qt::Window |
                   Qt::WindowStaysOnTopHint |
                   Qt::WindowDoesNotAcceptFocus |
                   Qt::WindowTitleHint |
                   Qt::CustomizeWindowHint |
                   Qt::WindowCloseButtonHint
                   );
}

RgbWidgets ColorInfoPanel::createPanel() {
    // Create a vertical layout for this specific panel
    QVBoxLayout* panelLayout = new QVBoxLayout();

    auto fileNameLabel = new QLabel(this);
    panelLayout->addWidget(fileNameLabel);

    // Create a horizontal layout for the color square and RGB labels
    QHBoxLayout* topLayout = new QHBoxLayout();

    // Create the RGB labels (R, G, B)
    auto rgbLabelsLayout = new QVBoxLayout();
    auto rLabel = new QLabel("R: 0", this);
    auto gLabel = new QLabel("G: 0", this);
    auto bLabel = new QLabel("B: 0", this);

    // Align the text to the center-left
    rLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    bLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Add RGB labels to the vertical layout
    rgbLabelsLayout->addWidget(rLabel);
    rgbLabelsLayout->addWidget(gLabel);
    rgbLabelsLayout->addWidget(bLabel);

    // Add a spacer to make sure labels are vertically aligned with the square
    rgbLabelsLayout->addStretch();

    // Create the color square (a fixed-size frame)
    auto colorSquare = new QFrame(this);
    colorSquare->setFixedSize(70, 70); // Set fixed size for the square
    colorSquare->setStyleSheet("background-color: rgb(0, 0, 0);"); // Default black color

    // Add RGB labels and color square to the horizontal layout
    topLayout->addLayout(rgbLabelsLayout);
    topLayout->addWidget(colorSquare);

    // Add the horizontal layout to the panel layout
    panelLayout->addLayout(topLayout);

    return { fileNameLabel, panelLayout, colorSquare, rLabel, gLabel, bLabel };
}

void ColorInfoPanel::updateTopPanel(RgbValue rgbValue) {
    // Set the file name in the top panel
    firstFileNameLabel->setText(rgbValue.getImageName());

    // Validate RGB values
    int r = rgbValue.getR();
    int g = rgbValue.getG();
    int b = rgbValue.getB();

    // Check if any RGB value is out of range
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        // Display "n/a" for invalid RGB values
        firstRLabel->setText("R: n/a");
        firstGLabel->setText("G: n/a");
        firstBLabel->setText("B: n/a");

        // Set a default background color (e.g., gray) for invalid RGB
        firstColorSquare->setStyleSheet("background-color: gray;");
    } else {
        // Update the background color of the square based on valid RGB values
        QString style = QString("background-color: rgb(%1, %2, %3);")
                            .arg(r)
                            .arg(g)
                            .arg(b);
        firstColorSquare->setStyleSheet(style);

        // Display valid RGB values
        firstRLabel->setText(QString("R: %1").arg(r));
        firstGLabel->setText(QString("G: %1").arg(g));
        firstBLabel->setText(QString("B: %1").arg(b));
    }
}

void ColorInfoPanel::updateBottomPanel(RgbValue rgbValue) {
    // If the panel is only for visible images, skip updating
    if (isForVisibleImageOnly) {
        return;
    }

    // Set the file name in the bottom panel
    secondFileNameLabel->setText(rgbValue.getImageName());

    // Validate RGB values
    int r = rgbValue.getR();
    int g = rgbValue.getG();
    int b = rgbValue.getB();

    // Check if any RGB value is out of range
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        // Display "n/a" for invalid RGB values
        secondRLabel->setText("R: n/a");
        secondGLabel->setText("G: n/a");
        secondBLabel->setText("B: n/a");

        // Set a default background color (e.g., gray) for invalid RGB
        secondColorSquare->setStyleSheet("background-color: gray;");
    } else {
        // Update the background color of the square based on valid RGB values
        QString style = QString("background-color: rgb(%1, %2, %3);")
                            .arg(r)
                            .arg(g)
                            .arg(b);
        secondColorSquare->setStyleSheet(style);

        // Display valid RGB values
        secondRLabel->setText(QString("R: %1").arg(r));
        secondGLabel->setText(QString("G: %1").arg(g));
        secondBLabel->setText(QString("B: %1").arg(b));
    }
}

void ColorInfoPanel::updateBothPanelsAndHighlightDifferences(RgbValue topRgb,
                                                             RgbValue bottomRgb,
                                                             bool showDifferenceForSmallerComponents
                                                             )
{
    if (isForVisibleImageOnly) {
        updateTopPanel(topRgb);
        return;
    }

    // Update the top panel
    firstFileNameLabel->setText(topRgb.getImageName());
    QString topStyle = QString("background-color: rgb(%1, %2, %3);")
                           .arg(topRgb.getR())
                           .arg(topRgb.getG())
                           .arg(topRgb.getB());
    firstColorSquare->setStyleSheet(topStyle);

    // Update the bottom panel
    secondFileNameLabel->setText(bottomRgb.getImageName());
    QString bottomStyle = QString("background-color: rgb(%1, %2, %3);")
                              .arg(bottomRgb.getR())
                              .arg(bottomRgb.getG())
                              .arg(bottomRgb.getB());
    secondColorSquare->setStyleSheet(bottomStyle);

    // Helper lambda to calculate and format differences for each RGB component
    auto formatComponent = [&](const QString& componentName, int topValue, int bottomValue, QLabel* topLabel, QLabel* bottomLabel) {
        QString reservedSpace = "   ";  // Reserve space for layout stability

        // Check if values are out of range
        if (topValue < 0 || topValue > 255 || bottomValue < 0 || bottomValue > 255) {
            // Display "n/a" for invalid values
            topLabel->setText(QString("%1: n/a").arg(componentName));
            bottomLabel->setText(QString("%1: n/a").arg(componentName));
            topLabel->setStyleSheet("color: black;");
            bottomLabel->setStyleSheet("color: black;");
            firstColorSquare->setStyleSheet("background-color: gray;");
            secondColorSquare->setStyleSheet("background-color: gray;");
            return;
        }

        // Calculate absolute difference
        int difference = abs(topValue - bottomValue);

        if (topValue > bottomValue) {
            // Top value is greater
            topLabel->setText(QString("%1: %2 | %3%4").arg(componentName).arg(topValue).arg(difference).arg(reservedSpace));
            if (showDifferenceForSmallerComponents) {
                bottomLabel->setText(QString("%1: %2 | %3%4").arg(componentName).arg(bottomValue).arg(difference).arg(reservedSpace));
            } else {
                bottomLabel->setText(QString("%1: %2").arg(componentName).arg(bottomValue));
            }
            topLabel->setStyleSheet("color: green;");
            bottomLabel->setStyleSheet("color: red;");
        } else if (topValue < bottomValue) {
            // Bottom value is greater
            if (showDifferenceForSmallerComponents) {
                topLabel->setText(QString("%1: %2 | %3%4").arg(componentName).arg(topValue).arg(difference).arg(reservedSpace));
            } else {
                topLabel->setText(QString("%1: %2").arg(componentName).arg(topValue));
            }
            bottomLabel->setText(QString("%1: %2 | %3%4").arg(componentName).arg(bottomValue).arg(difference).arg(reservedSpace));
            topLabel->setStyleSheet("color: red;");
            bottomLabel->setStyleSheet("color: green;");
        } else {
            // Values are equal, no difference to show
            topLabel->setText(QString("%1: %2").arg(componentName).arg(topValue));
            bottomLabel->setText(QString("%1: %2").arg(componentName).arg(bottomValue));
            topLabel->setStyleSheet("color: black;");
            bottomLabel->setStyleSheet("color: black;");
        }
    };

    // Update and compare R values
    formatComponent("R", topRgb.getR(), bottomRgb.getR(), firstRLabel, secondRLabel);

    // Update and compare G values
    formatComponent("G", topRgb.getG(), bottomRgb.getG(), firstGLabel, secondGLabel);

    // Update and compare B values
    formatComponent("B", topRgb.getB(), bottomRgb.getB(), firstBLabel, secondBLabel);
}
