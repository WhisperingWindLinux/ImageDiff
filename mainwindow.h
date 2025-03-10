#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <interfaces/mainwindowcallbacks.h>
#include <interactors/comparisioninteractor.h>


class ImageViewer;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public AMainWindowCallbacks
{
    Q_OBJECT

public slots:
    void actionCloseImages_triggered();
    void actionOpenImages_triggered();
    void actionSwitchBetweenImages_triggered();
    void actionImageComparatorsMenuItem_triggered();
    void actionSaveImageAs_triggered();
    void actionSaveVisibleAreaAs_triggered();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showStatusMessage(QString message);

private:
    Ui::MainWindow *ui;
    ImageViewer *viewer = nullptr;
    ComparisionInteractor *comparisionInteractor = nullptr;

    void buildMenus();
    bool loadImagesBeingCompared();
    void showError(const QString &errorMessage);

    // AMainWindowCallbacks interface
public:
    void onImagesBeingComparedLoaded(QPixmap& image1, QString path1, QPixmap& image2, QString path2);
    void onComparisonImagesLoaded(QPixmap &image, QString description);
    void onComparisonTextLoaded(QString text);
    void saveImageAs(QPixmap &image, QString defaultPath);

};
#endif // MAINWINDOW_H
