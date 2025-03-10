#ifndef COLORINFOPANEL_H
#define COLORINFOPANEL_H

#include <QObject>
#include <QWidget>
#include <qboxlayout.h>
#include <qframe.h>
#include <qlabel.h>

class ColorInfoPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ColorInfoPanel(QWidget *parent = nullptr);

    void updateColor(QString imageName, int r, int g, int b);

private:
    QFrame* colorSquare;       // The square that shows the color
    QLabel* rLabel;            // Label for R value
    QLabel* gLabel;            // Label for G value
    QLabel* bLabel;            // Label for B value
    QVBoxLayout* rgbLabelsLayout; // Layout for RGB labels

};

#endif // COLORINFOPANEL_H
