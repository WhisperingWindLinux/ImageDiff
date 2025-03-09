#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ImageViewer;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void actionCloseImages_triggered();
    void actionOpenImages_triggered();
    void actionShowAbsolutePixelsValueDifference_triggered();
    void actionShowDifferenceAsImage_triggered();
    void actionShowPixelsBrigthnessDifference_triggered();
    void actionShowPixelsSaturationDifference_triggered();
    void actionShowPixeslContrastDifference_triggered();
    void actionSwitchBetweenImages_triggered();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showStatusMessage(QString &message);

private:
    Ui::MainWindow *ui;
    ImageViewer *viewer;
};
#endif // MAINWINDOW_H
