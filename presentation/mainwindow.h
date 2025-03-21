#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QProcess>
#include <QProgressDialog>
#include <domain/interfaces/colorundercursorchangelistener.h>
#include <domain/interfaces/iprogressdialog.h>
#include <domain/interfaces/processorpropertiesdialogcallback.h>
#include <business/imageanalysis/imageprocessinginteractor.h>
#include "imageprocessorsmenucontroller.h"


class ImageViewer;
class ColorPickerPanel;
class ColorPickerController;
class RecentFilesInteractor;
class ImageProcessingInteractor;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow,
                   public IProgressDialog,
                   public IColorUnderCursorChangeListener,
                   public IPropcessorPropertiesDialogCallback
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

    void showProgressDialog(const QString &caption, int totalSteps) override;
    bool wasCanceled() override;
    void onUpdateProgressValue(int value) override;
    void onMessage(const QString &message) override;
    void onError(const QString &error) override;

    // IMainWindow interface
    // TBD Refactoring


protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool event(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    ImageViewer *imageView;
    ImageProcessingInteractor *comparisionInteractor;
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

