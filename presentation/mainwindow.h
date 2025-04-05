#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QProcess>
#include <QProgressDialog>
#include <domain/interfaces/presentation/idroptarget.h>
#include <domain/interfaces/presentation/iotherappinstancesinteractorcallback.h>
#include <domain/interfaces/presentation/icolorundercursorchangelistener.h>
#include <domain/interfaces/presentation/iprogressdialog.h>
#include <domain/interfaces/presentation/ioncropimageslistener.h>
#include <domain/interfaces/presentation/iprocessorpropertiesdialogcallback.h>
#include <business/imageanalysis/imageprocessinginteractor.h>
#include <business/imagefilesinteractors.h>
#include "imageprocessorsmenucontroller.h"


class ImageViewer;
class IColorPickerController;
class RecentFilesInteractor;
class ImageProcessingInteractor;
class OtherAppInstancesInteractor;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow,
                   public IProgressDialog,
                   public IColorUnderCursorChangeListener,
                   public IPropcessorPropertiesDialogCallback,
                   public IImageFilesInteractorListener,
                   public IImageProcessingInteractorListener,
                   public IDropListener,
                   public OnCropImageListener,
                   public OtherAppInstancesInteractorCallback
{
    Q_OBJECT

public slots:
    void closeImages();
    void openImages();
    void openImage();
    void switchBetweenImages();
    void callImageProcessor();
    void callImageProcessorsHelp();
    void saveImageAs();
    void saveVisibleAreaAs();
    void showAboutDialog();
    void showDockedColorPicker();
    void reloadImagesFromDisk();
    void imageZoomedToActualSize();
    void imageZoomIn();
    void imageZoomOut();
    void openRecentFile();
    void placeColorPickerOnRight();
    void placeColorPickerOnLeft();
    void imagFitInView();
    void getImagesFromVideos();
    void runAllComparators();
    void changePluginsSettings();
    void rescanPluginDir();
    void clearOpenRecentsMenu();
    void showFirstImage();
    void showSecondImage();
    void showComparisonImage();
    void showImageAutoAnalysisSettings();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showStatusMessage(QString message);
    void openImagesInOtherAppInstance(QString firstFilePath, QString secondFilePath);
    void openImagesFromCommandLine(const QString &firstFilePath, const QString &secondFilePath);
    void openImageFromCommandLine(const QString &filePath);
    void onColorUnderCursorTrackingStatusChanged(bool isActive);
    void onSelectedAreaShouldBeAnalyzed(ImagesPtr images, std::optional<int> key);

    // IProgressDialog interface

    void showProgressDialog(const QString &caption, int totalSteps) override;
    bool wasCanceled() override;
    void onUpdateProgressValue(int value) override;
    void onMessage(const QString &message) override;
    void onError(const QString &error) override;

    // IColorUnderCursorChangeListener interface

    void onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                   const ImagePixelColor &hiddenImageRgbValue) override;

    // IPropcessorPropertiesDialogCallback intergace

    QList<Property> showImageProcessorPropertiesDialog(const QString &processorName,
                                                       const QString &processorDescription,
                                                       const QList<Property> &defaultProperties) override;


    // IImageFilesInteractorListener interface

    void onImagesOpened(const ImagesPtr images) override;
    void onImagesOpenFailed(const QString &error) override;
    void onImagesClosed() override;
    void onSavingFileFailed(const QString &path) override;
    void onFileSavedSuccessfully(const QString &path) override;

    // IImageProcessingInteractorListener interface

    void onComparisonResultLoaded(const QPixmap &image, const QString &description) override;

    void onComparisonResultLoaded(const QString &html, const QString &comparatorFullName,
                                  const QString &firstImagePath,
                                  const QString &secondImagePath) override;

    void onShowImageInExternalViewer(const QPixmap &image, const QString &description) override;
    void onFilteredResultLoaded(const QPixmap &firstImage, const QPixmap &secondImage) override;
    void onImageProcessorFailed(const QString &error) override;
    void onFastSwitchingToComparisonImageStatusChanged(bool isSwitchingAvailable) override;

    // IDropListener interface

    void onDrop(QList<QUrl> urls) override;

    // OnCropImageListener interface

    void onImagesCropped(ImagesPtr images) override;

    // OpenImagesInOtherAppInstanceInteractorCallback interface

    void onOtherAppInstanceOpened() override;
    void showError(const QString &errorMessage) override;

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void showEvent(QShowEvent *) override;

private:
    Ui::MainWindow *ui;
    ImageViewer *imageView;
    ImageFilesInteractor *imageFilesInteractor;
    ImageProcessingInteractor *imageProcessingInteractor;
    IColorPickerController *colorPickerController;
    ImageProcessorsMenuController *imageProcessorsMenuController;
    RecentFilesInteractor *recentFilesInteractor;
    OtherAppInstancesInteractor *otherAppInstanceInteractor;
    QProgressDialog *progressDialog;

    // The application was not originally designed for opening a single image.
    // This flag adds that feature. Under the hood, the image is still
    // opened as two images for comparison, but the process of opening it
    // is simplified. For example, the open dialog, drag-and-drop, and
    // opening via double-click from Finder become available for a single image.
    // The feature is disabled by default as it might confuse users. Enable it
    // only if you are sure you need it and understand how it works.
    // To open an image from Finder by double-clicking, you have to uncomment
    //          # QMAKE_INFO_PLIST = Info.plist
    // in ImageDiff.pro

    const bool isOpenSingleImageEnabledFeatureFlag;

    void buildImageProcessorsMenu();
    void makeConnections();
    void loadTwoImagesBeingCompared();
    void enableImageProceesorsMenuItems(bool isEnabled);
    void saveMainWindowPosition();
    void restoreMainWindowPosition();
    void updateRecentFilesMenu();
};
#endif // MAINWINDOW_H

