#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <interfaces/mainwindowcallbacks.h>
#include <interactors/comparisoninteractor.h>
#include <interactors/recentfilesmanager.h>
#include <interactors/rgbtrackinginteractor.h>
#include <gui/RgbValue.h>

#include <QMainWindow>
#include <QProcess>

class ImageViewer;
class ColorInfoPanel;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public IMainWindowCallbacks
{
    Q_OBJECT

public slots:
    void closeImages();
    void openImages();
    void switchBetweenImages();
    void callImageComparator();
    void saveImageAs();
    void saveVisibleAreaAs();
    void showAboutDialog();
    void showColorPicker();
    void showAdvancedColorPicker();
    void showOriginalImages();
    void imageZoomedToActualSize();
    void imageZoomIn();
    void imageZoomOut();
    void openRecentFile();
    void placeColorPickerOnRight();
    void placeColorPickerOnLeft();
    void imagFitInView();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showStatusMessage(QString message);
    void passTwoImagesBeingComparedToOtherAppInstance(QString firstFilePath, QString secondFilePath);
    void onReceiveTwoImagesBeingComparedViaCommandline(QString firstFilePath, QString secondFilePath);
    void onRgbTrackingStatusChanged(bool isActive);


protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool event(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    ImageViewer *viewer = nullptr;
    ComparisonInteractor* comparisionInteractor = nullptr;
    RgbTrackingInteractor *rgbTrackingInteractor = nullptr;
    QList<shared_ptr<QProcess>> instances;

    void buildMenu();
    void makeConnections();
    void loadTwoImagesBeingCompared();
    void showError(const QString &errorMessage);
    void openColorPickerDialog(bool isOnePanelMode);
    void closeColorPickerDialog();
    void enabledImageOperationMenuItems(bool isEnabled);
    void deleteImageView();
    void createImageView();
    void positionColorPickerWindow(QWidget *colorPickerWindow, int alignmentPercent, bool placeOnRight);
    void saveMainWindowPosition();
    void restoreMainWindowPosition();

    // AMainWindowCallback interface
public:
    void onTwoImagesBeingComparedLoadedSuccessfully(QPixmap& image1,
                                     QString path1,
                                     QPixmap& image2,
                                     QString path2,
                                     bool usePreviousImageGeometry) override;

    void onImageResultFromComparatorReceived(QPixmap &image, QString description) override;
    void onTextResultFromComparatorReceived(QString text) override;
    void saveImage(QPixmap &image, QString defaultPath) override;
    void onRgbValueUnderCursonChanged(RgbValue firstImageRgbValue, RgbValue secondImageRgbValue) override;
    QList<Property> getUpdatedPropertiesFromUser(QString processorName,
                                                 QString processorDescription,
                                                 QList<Property> defaultProperties) override;
    void updateRecentFilesMenu() override;
};
#endif // MAINWINDOW_H

