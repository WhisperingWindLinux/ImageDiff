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
    void openImageFromClipboard();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openImagesInOtherAppInstance(QString firstFilePath, QString secondFilePath);
    void openImagesFromCommandLine(const QString &firstFilePath, const QString &secondFilePath);
    void openImageFromCommandLine(const QString &filePath);
    void onColorUnderCursorTrackingStatusChanged(bool isActive);
    void onSelectedAreaShouldBeAnalyzed(ImageHolderPtr images, std::optional<int> key);

    void onComparebleImageDisplayed(const QString &imageName);
    void onComparisonImageDisplayed(const QString &image1Name,
                                    const QString &image2Name,
                                    const QString &comparatorName
                                    );

    // IProgressDialog interface

    void showProgressDialog(const QString &caption, int totalSteps) override;
    bool wasCanceled() override;
    void onUpdateProgressValue(int value) override;
    void onMessage(const QString &message) override;
    void onError(const QString &error) override;

    // IColorUnderCursorChangeListener interface

    void onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                   const std::optional<ImagePixelColor> &hiddenImageRgbValue) override;

    // IPropcessorPropertiesDialogCallback intergace

    QList<Property> showImageProcessorPropertiesDialog(const QString &processorName,
                                                       const QString &processorDescription,
                                                       const QList<Property> &defaultProperties) override;


    // IImageFilesInteractorListener interface

    void onImagesOpened(const ImageHolderPtr images) override;
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
    void onFilteredResultLoaded(const ImageHolderPtr imageHolder) override;
    void onImageProcessorFailed(const QString &error) override;
    void onFastSwitchingToComparisonImageStatusChanged(bool isSwitchingAvailable) override;

    // IDropListener interface

    void onDrop(QList<QUrl> urls) override;

    // OnCropImageListener interface

    void onImagesCropped(ImageHolderPtr images) override;

    // OpenImagesInOtherAppInstanceInteractorCallback interface

    void onOtherAppInstanceOpened() override;
    void showError(const QString &errorMessage) override;

    void mousePressEvent(QMouseEvent *event) override;

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void showEvent(QShowEvent *) override;

private:
    Ui::MainWindow *ui;
    ImageViewer *mImageView;
    ImageFilesInteractor *mImageFilesInteractor;
    ImageProcessingInteractor *mImageProcessingInteractor;
    IColorPickerController *mColorPickerController;
    ImageProcessorsMenuController *mImageProcessorsMenuController;
    RecentFilesInteractor *mRecentFilesInteractor;
    OtherAppInstancesInteractor *mOtherAppInstanceInteractor;
    QProgressDialog *mProgressDialog;
    bool mMenuIsInSingleImageMode;

    void buildImageProcessorsMenu();
    void makeConnections();
    void loadTwoImagesBeingCompared();
    void enableImageProceesorsMenuItems(bool isEnabled);
    void saveMainWindowPosition();
    void restoreMainWindowPosition();
    void updateRecentFilesMenu();
};
#endif // MAINWINDOW_H

