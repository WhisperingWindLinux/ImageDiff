#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <interfaces/mainwindowcallbacks.h>
#include <interactors/comparisoninteractor.h>
#include <interactors/recentfilesmanager.h>
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
    void zoomIn();
    void zoomOut();
    void openRecentFile();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showStatusMessage(QString message);

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::MainWindow *ui;
    ImageViewer *viewer = nullptr;
    ComparisonInteractor* comparisionInteractor = nullptr;
    ColorInfoPanel* colorPanel = nullptr;

    void populateComparatorsAndFiltersMenus();
    void loadTwoImagesBeingCompared();
    void showError(const QString &errorMessage);
    void openColorPickerDialog(bool isOnePanelMode);
    void closeColorPickerDialog();
    void disabledMenusIfImagesNotOpened();
    void enabledMenusIfImagesOpened();
    void deleteImageView();
    void createImageView();

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

