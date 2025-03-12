#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <interfaces/amainwindowcallbacks.h>
#include <interactors/comparisoninteractor.h>
#include <gui/RgbValue.h>

#include <QMainWindow>

class ImageViewer;
class ColorInfoPanel;

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
    void actionAbout_triggered();
    void actionColorPicker_triggered();
    void actionShowAdvancedColorPicker_triggered();
    void actionShowOriginalImage_triggered();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showStatusMessage(QString message);

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    Ui::MainWindow *ui;
    ImageViewer *viewer = nullptr;
    ComparisonInteractor *comparisionInteractor = nullptr;
    bool isColorPickerActive = false;
    ColorInfoPanel* colorPanel = nullptr;

    void buildMenus();
    bool loadImagesBeingCompared();
    void showError(const QString &errorMessage);
    void openColorPickerDialog(bool isForVisibleImageOnly);
    void closeColorPickerDialog();

public:
    void onImagesBeingComparedLoaded(QPixmap& image1,
                                     QString path1,
                                     QPixmap& image2,
                                     QString path2,
                                     bool usePreviousImageGeometry) override;

    void onComparisonImagesLoaded(QPixmap &image, QString description) override;
    void onComparisonTextLoaded(QString text) override;
    void saveImageAs(QPixmap &image, QString defaultPath) override;
    void onRgbValueUnderCursonChanged(RgbValue firstImageRgbValue, RgbValue secondImageRgbValue) override;
    QList<Property> getUpdatedPropertiesFromUser(QList<Property> defaultProperties) override;
};
#endif // MAINWINDOW_H

