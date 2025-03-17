#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindowbuildmenudelegate.h"

#include <QMainWindow>
#include <QProcess>
#include <QProgressDialog>
#include <domain/interfaces/mainwindowcallbacks.h>
#include <business/imageanalysis/comparisoninteractor.h>

class ImageViewer;
class ColorInfoPanel;
class RgbTrackingHelper;

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
    void callImageProcessor();
    void callImageProcessorHelp();
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
    void grabImagesFromVideos();
    void runAllComparators();

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
    ImageViewer *viewer;
    ComparisonInteractor *comparisionInteractor;
    RgbTrackingHelper *rgbTrackingInteractor;
    MainWindowBuildMenuDelegate *buildMenuDelegate;
    QProgressDialog *progressDialog;
    QList<shared_ptr<QProcess> > instances;

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

    void onTextResultFromComparatorReceived(QString &message,
                                            QString comparatorFullName,
                                            QString comparatorDescription,
                                            QString &firstImageFilePath,
                                            QString &secondImageFilePath) override;

    void userShouldSeeHelpMessage(QString &message) override;

    void saveImage(QPixmap &image, QString defaultPath) override;

    void onRgbValueUnderCursonChanged(RgbValue firstImageRgbValue, RgbValue secondImageRgbValue) override;

    QList<Property> getUpdatedPropertiesFromUser(QString processorName,
                                                 QString processorDescription,
                                                 QList<Property> defaultProperties) override;
    void updateRecentFilesMenu() override;

    // IProgressDialog interface
public:
    void showProgressDialog(QString caption, int totalSteps) override;
    bool wasCanceled() override;
    void onUpdateProgressValue(int value) override;
    void onMessage(QString message) override;
    void onError(QString error) override;
};
#endif // MAINWINDOW_H

