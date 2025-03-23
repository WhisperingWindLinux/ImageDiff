#include "colorpickerpanel.h"

#include <qboxlayout.h>
#include <qlineedit.h>

ColorPickerPanel::ColorPickerPanel(bool isForRightPosition, QWidget *parent, bool isTwoPanelMode)
    : QWidget(parent),
    isTwoPanelMode(isTwoPanelMode)
{
    setWindowTitle("Color Picker");
    setLayout(isForRightPosition);
    reset();

    setWindowFlags(Qt::Window |
                   Qt::WindowStaysOnTopHint |
                   Qt::WindowDoesNotAcceptFocus |
                   Qt::WindowTitleHint |
                   Qt::CustomizeWindowHint |
                   Qt::WindowCloseButtonHint
                   );
}

void ColorPickerPanel::setLayout(bool isForRightPosition) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Create the first panel (visible image panel)
    auto rgbWidgets = createPanel(isForRightPosition);
    mainLayout->addLayout(rgbWidgets.panelLayout);
    firstFileNameLabel = rgbWidgets.fileNamelabel;
    firstColorSquare = rgbWidgets.colorSquare;
    firstRLabel = rgbWidgets.rLabel;
    firstGLabel = rgbWidgets.gLabel;
    firstBLabel = rgbWidgets.bLabel;

    // If isTwoPanelMode is true, create a advanced color picker' panel
    if (isTwoPanelMode) {
        // Add some spacing before the second panel
        mainLayout->addSpacing(20);

        // Create the second panel
        auto rgbWidgets = createPanel(isForRightPosition);

        mainLayout->addLayout(rgbWidgets.panelLayout);

        // Store references to the second panel's elements
        secondFileNameLabel = rgbWidgets.fileNamelabel;
        secondColorSquare = rgbWidgets.colorSquare;
        secondRLabel = rgbWidgets.rLabel;
        secondGLabel = rgbWidgets.gLabel;
        secondBLabel = rgbWidgets.bLabel;
    }

    // Add a spacer to push everything to the top of the panel
    mainLayout->addStretch(1000);
    mainLayout->addSpacing(10);
}

RgbWidgets ColorPickerPanel::createPanel(bool isForRightPosition) {
    // Create a vertical layout for this specific panel
    QVBoxLayout *panelLayout = new QVBoxLayout();

    auto fileNameLabel = new QLabel(this);
    panelLayout->addWidget(fileNameLabel);

    // Create a horizontal layout for the color square and RGB labels
    QHBoxLayout* topLayout = new QHBoxLayout();

    // Create the RGB labels (R, G, B)
    auto rgbLabelsLayout = new QVBoxLayout();
    auto rLabel = new QLabel("R: 0", this);
    auto gLabel = new QLabel("G: 0", this);
    auto bLabel = new QLabel("B: 0", this);
    rLabel->setMinimumWidth(85);
    gLabel->setMinimumWidth(85);
    bLabel->setMinimumWidth(85);

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
    auto colorSquareLayout = new QVBoxLayout();
    auto colorSquare = new QFrame(this);
    colorSquare->setFixedSize(70, 70); // Set fixed size for the square
    colorSquare->setStyleSheet("background-color: rgb(0, 0, 0);"); // Default black color
    colorSquareLayout->addWidget(colorSquare);
    colorSquareLayout->addStretch();

    // Add RGB labels and color square to the horizontal layout
    if (isForRightPosition) {
        topLayout->addLayout(rgbLabelsLayout);
        topLayout->addLayout(colorSquareLayout);
    } else {
        colorSquareLayout->setContentsMargins(0, 0, 10, 0);
        topLayout->addLayout(colorSquareLayout);
        topLayout->addLayout(rgbLabelsLayout);
    }
    topLayout->addStretch();

    // Add the horizontal layout to the panel layout
    panelLayout->addLayout(topLayout);

    return { fileNameLabel, panelLayout, colorSquare, rLabel, gLabel, bLabel };
}

void ColorPickerPanel::reset() {
    ImagePixelColor emptyData =  { "n/a", -1, -1, -1 };
    update(emptyData, emptyData);
}

void ColorPickerPanel::updateTopPanelOnly(const ImagePixelColor &visibleImageColor) {
    // Set the file name in the top panel
    firstFileNameLabel->setText(visibleImageColor.getImageName());

    // Validate RGB values
    int r = visibleImageColor.getR();
    int g = visibleImageColor.getG();
    int b = visibleImageColor.getB();

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

void ColorPickerPanel::update(const ImagePixelColor &visibleImageColor,
                              std::optional<ImagePixelColor> hiddenImageColor
                             )
{
    if (!isTwoPanelMode) {
        updateTopPanelOnly(visibleImageColor);
        return;
    }

    if (!hiddenImageColor) {
        reset();
    }

    // Update the top panel
    firstFileNameLabel->setText(visibleImageColor.getImageName());
    QString topStyle = QString("background-color: rgb(%1, %2, %3);")
                           .arg(visibleImageColor.getR())
                           .arg(visibleImageColor.getG())
                           .arg(visibleImageColor.getB());
    firstColorSquare->setStyleSheet(topStyle);

    // Update the bottom panel
    secondFileNameLabel->setText(hiddenImageColor->getImageName());
    QString bottomStyle = QString("background-color: rgb(%1, %2, %3);")
                              .arg(hiddenImageColor->getR())
                              .arg(hiddenImageColor->getG())
                              .arg(hiddenImageColor->getB());
    secondColorSquare->setStyleSheet(bottomStyle);

    // Helper lambda to calculate and format differences for each RGB component
    auto formatComponent = [&](const QString& componentName,
                               int topValue,
                               int bottomValue,
                               QLabel* topLabel,
                               QLabel* bottomLabel
                               )
    {
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
            topLabel->setText(format(componentName, topValue, difference));
            bottomLabel->setText(format(componentName, bottomValue, difference));
            topLabel->setStyleSheet("color: green;");
            bottomLabel->setStyleSheet("color: red;");
        } else if (topValue < bottomValue) {
            // Bottom value is greater
            topLabel->setText(format(componentName, topValue, difference));
            bottomLabel->setText(format(componentName, bottomValue, difference));
            topLabel->setStyleSheet("color: red;");
            bottomLabel->setStyleSheet("color: green;");
        } else {
            // Values are equal, no difference to show
            topLabel->setText(format(componentName, topValue, difference));
            bottomLabel->setText(format(componentName, bottomValue, difference));
            topLabel->setStyleSheet("color: black;");
            bottomLabel->setStyleSheet("color: black;");
        }
    };

    // Update and compare R values
    formatComponent("R",
                    visibleImageColor.getR(),
                    hiddenImageColor->getR(),
                    firstRLabel,
                    secondRLabel
                    );

    // Update and compare G values
    formatComponent("G",
                    visibleImageColor.getG(),
                    hiddenImageColor->getG(),
                    firstGLabel,
                    secondGLabel
                    );

    // Update and compare B values
    formatComponent("B",
                    visibleImageColor.getB(),
                    hiddenImageColor->getB(),
                    firstBLabel,
                    secondBLabel
                    );
}

QString ColorPickerPanel::format(const QString &colorComponemt,
                                 int color,
                                 int diff,
                                 bool alignColorValueLeft,
                                 bool alignDiffValueLeft
                                 )
{
    const int maxNumberWidth = 3;
    const QString separator = "  |  ";

    QString strColor = QString::number(color);

    QString formattedColor = alignColorValueLeft ? strColor.leftJustified(maxNumberWidth, ' ') :
                             strColor.rightJustified(maxNumberWidth, ' ');

    if (diff == 0) {
        return QString(colorComponemt) + ":  " + formattedColor;
    } else {
        QString strDiff = QString::number(diff);
        QString formattedDiff = alignDiffValueLeft ? strDiff.leftJustified(maxNumberWidth, ' ') :
                                    strDiff.rightJustified(maxNumberWidth, ' ');

        return QString(colorComponemt) + ":  " + formattedColor + separator + formattedDiff;
    }


}














