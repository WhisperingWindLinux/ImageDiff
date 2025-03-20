#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imageprocessorsmenucontroller.h"

#include <QMainWindow>
#include <QProcess>
#include <QProgressDialog>
#include <domain/interfaces/mainwindowcallbacks.h>
#include <business/imageanalysis/comparisoninteractor.h>

class ImageViewer;
class ColorPickerPanel;
class ColorPickerController;
class RecentFilesInteractor;

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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showStatusMessage(QString message);
    void openImagesInOtherAppInstance(QString firstFilePath, QString secondFilePath);
    void openImagesFromCommandLine(QString firstFilePath, QString secondFilePath);
    void onColorUnderCursorTrackingStatusChanged(bool isActive);

    // IProgressDialog interface

    void showProgressDialog(QString caption, int totalSteps) override;
    bool wasCanceled() override;
    void onUpdateProgressValue(int value) override;
    void onMessage(QString message) override;
    void onError(QString error) override;

    // IMainWindow interface

    void displayImages(const std::shared_ptr<QPixmap> image1,
                       const QString &path1,
                       const std::shared_ptr<QPixmap> image2,
                       const QString &path2) override;

    void onColorUnderCursorChanged(ImagePixelColor visibleImageRgbValue,
                                   ImagePixelColor hiddenImageRgbValue) override;

    void onImageResultFromComparatorReceived(const std::shared_ptr<QPixmap>  image,
                                             const QString &description) override;

    void onTextResultFromComparatorReceived(const QString &message,
                                            const QString &comparatorFullName,
                                            const QString &comparatorDescription,
                                            const QString &firstImageFilePath,
                                            const QString &secondImageFilePath) override;

    void saveImage(const std::shared_ptr<QPixmap>  image, const QString &defaultPath) override;

    void showHelp(const QString &message) override;

    QList<Property> showImageProcessorPropertiesDialog(const QString &processorName,
                                                       const QString &processorDescription,
                                                       const QList<Property> &defaultProperties) override;
    void updateRecentFilesMenu() override;

    void onDisplayedImagesShouldBeReplaced(const std::shared_ptr<QPixmap> first,
                                           const std::shared_ptr<QPixmap> second) override;

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool event(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    ImageViewer *imageView;
    ComparisonInteractor *comparisionInteractor;
    ColorPickerController *colorPickerController;
    ImageProcessorsMenuController *imageProcessorsMenuController;
    RecentFilesInteractor *recentFilesInteractor;
    QProgressDialog *progressDialog;
    QList<shared_ptr<QProcess> > instances;

    void buildImageProcessorsMenu();
    void makeConnections();
    void loadTwoImagesBeingCompared();
    void showError(const QString &errorMessage);
    void enableImageProceesorsMenuItems(bool isEnabled);
    void saveMainWindowPosition();
    void restoreMainWindowPosition();
    void coreUpdateRecentFilesMenu();
};
#endif // MAINWINDOW_H

