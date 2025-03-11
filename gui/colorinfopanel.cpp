#include "colorinfopanel.h"

#include <qboxlayout.h>

ColorInfoPanel::ColorInfoPanel(QWidget *parent)
    : QWidget{parent}
{
    // Create the main layout for the panel (vertical layout)
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10); // Add padding
    mainLayout->setSpacing(10); // Add spacing between elements

    // Create a horizontal layout for the color square and RGB labels
    QHBoxLayout* topLayout = new QHBoxLayout();

    // Create the RGB labels (R, G, B)
    rgbLabelsLayout = new QVBoxLayout();
    rLabel = new QLabel("R: 0", this);
    gLabel = new QLabel("G: 0", this);
    bLabel = new QLabel("B: 0", this);

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
    colorSquare = new QFrame(this);
    colorSquare->setFixedSize(70, 70); // Set fixed size for the square
    colorSquare->setStyleSheet("background-color: rgb(0, 0, 0);"); // Default black color

    // Add RGB labels and color square to the horizontal layout
    topLayout->addLayout(rgbLabelsLayout);
    topLayout->addWidget(colorSquare);

    // Add the horizontal layout to the main layout
    mainLayout->addLayout(topLayout);

    // Add a spacer to push everything to the top of the panel
    mainLayout->addStretch();
}


void ColorInfoPanel::updateColor(QString imageName, int r, int g, int b) {
    if (imageName.isEmpty()) {
        setWindowTitle("");
    } else {
        setWindowTitle(imageName);
    }
    QString style = QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b);
    colorSquare->setStyleSheet(style); // Update the background color of the square
    rLabel->setText(QString("R: %1").arg(r));
    gLabel->setText(QString("G: %1").arg(g));
    bLabel->setText(QString("B: %1").arg(b));
}
