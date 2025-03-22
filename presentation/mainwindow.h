#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QProcess>
#include <QProgressDialog>
#include <domain/interfaces/IDropTarget.h>
#include <domain/interfaces/otherappinstancesinteractorcallback.h>
#include <domain/interfaces/colorundercursorchangelistener.h>
#include <domain/interfaces/iprogressdialog.h>
#include <domain/interfaces/oncropimageslistener.h>
#include <domain/interfaces/processorpropertiesdialogcallback.h>
#include <business/imageanalysis/imageprocessinginteractor.h>
#include <business/imagefilesinteractors.h>
#include "imageprocessorsmenucontroller.h"


class ImageViewer;
class ColorPickerPanel;
class ColorPickerController;
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
    void switchBetweenImages();
    void callImageProcessor();
    void callImageProcessorsHelp();
    void saveImageAs();
    void saveVisibleAreaAs();
    void showAboutDialog();
    void showColorPicker();
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
    void changePluginsSettings();
    void rescanPluginDir();
    void clearOpenRecentsMenu();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showStatusMessage(QString message);
    void openImagesInOtherAppInstance(QString firstFilePath, QString secondFilePath);
    void openImagesFromCommandLine(const QString &firstFilePath, const QString &secondFilePath);
    void onColorUnderCursorTrackingStatusChanged(bool isActive);

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

    void onShowImageProcessorsHelp(const QString &html) override;
    void onComparisonResultLoaded(const QPixmap &image, const QString &description) override;

    void onComparisonResultLoaded(const QString &html, const QString &comparatorFullName,
                                  const QString &comparatorDescription,
                                  const QString &firstImagePath,
                                  const QString &secondImagePath) override;

    void onFilteredResultLoaded(const QPixmap &firstImage, const QPixmap &secondImage) override;
    void onImageProcessorFailed(const QString &error) override;

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
    bool event(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    ImageViewer *imageView;
    ImageFilesInteractor *imageFilesInteractor;
    ImageProcessingInteractor *imageProcessingInteractor;
    ColorPickerController *colorPickerController;
    ImageProcessorsMenuController *imageProcessorsMenuController;
    RecentFilesInteractor *recentFilesInteractor;
    OtherAppInstancesInteractor *otherAppInstanceInteractor;
    QProgressDialog *progressDialog;
    QList<shared_ptr<QProcess> > instances;

    void buildImageProcessorsMenu();
    void makeConnections();
    void loadTwoImagesBeingCompared();
    void enableImageProceesorsMenuItems(bool isEnabled);
    void saveMainWindowPosition();
    void restoreMainWindowPosition();
    void updateRecentFilesMenu();    
};
#endif // MAINWINDOW_H

