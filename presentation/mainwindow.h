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

public:
    void displayImages(QPixmap& image1,
                       QString path1,
                       QPixmap& image2,
                       QString path2) override;

    void onImageResultFromComparatorReceived(QPixmap &image, QString description) override;

    void onTextResultFromComparatorReceived(QString &message,
                                            QString comparatorFullName,
                                            QString comparatorDescription,
                                            QString &firstImageFilePath,
                                            QString &secondImageFilePath) override;

    void showHelp(QString &message) override;

    void saveImage(QPixmap &image, QString defaultPath) override;

    void onColorUnderCursorChanged(ImagePixelColor firstImageRgbValue,
                                      ImagePixelColor secondImageRgbValue) override;

    QList<Property> showImageProcessorPropertiesDialog(QString processorName,
                                                 QString processorDescription,
                                                 QList<Property> defaultProperties) override;
    void updateRecentFilesMenu() override;

    void onDisplayedImagesShouldBeReplaced(QPixmap& first, QPixmap &second) override;

    // IProgressDialog interface
public:
    void showProgressDialog(QString caption, int totalSteps) override;
    bool wasCanceled() override;
    void onUpdateProgressValue(int value) override;
    void onMessage(QString message) override;
    void onError(QString error) override;
};
#endif // MAINWINDOW_H

