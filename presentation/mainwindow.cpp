#include "mainwindow.h"
#include "business/recentfilesinteractor.h"

#include <QLabel>
#include <QSplitter>
#include <QVBoxLayout>
#include <qgraphicsview.h>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QPixmap>
#include <QScrollBar>
#include <QDebug>
#include <qmessagebox.h>
#include <QDockWidget>
#include <QMimeData>
#include <qprocess.h>
#include <ui_mainwindow.h>
#include <business/getimagesfromvideosinteractor.h>
#include <presentation/views/imageviewer.h>
#include <presentation/dialogs/aboutdialog.h>
#include <presentation/dialogs/comparatorresultdialog.h>
#include <presentation/dialogs/helpdialog.h>
#include <presentation/dialogs/pluginssettingsdialog.h>
#include <presentation/dialogs/propertyeditordialog.h>
#include <data/storage/savefiledialoghandler.h>
#include <presentation/colorpickercontroller.h>
#include <business/imageanalysis/imageprocessinginteractor.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    setWindowTitle("Image Diff");
    resize(1380, 820);
    showNormal();
    restoreMainWindowPosition();

    imageView = new ImageViewer(this, this);
    setCentralWidget(imageView);

    imageFilesInteractor = new ImageFilesInteractor();
    colorPickerController = new ColorPickerController(this);
    imageProcessorsMenuController = new ImageProcessorsMenuController(this);
    recentFilesInteractor = new RecentFilesInteractor();
    imageProcessingInteractor = nullptr;
    progressDialog = nullptr;

    imageFilesInteractor->subscribe(this);

    buildImageProcessorsMenu();
    makeConnections();
    enableImageProceesorsMenuItems(false);

    updateRecentFilesMenu();
}

MainWindow::~MainWindow() {
    delete ui;
}

/*  Application menu settings { */

void MainWindow::buildImageProcessorsMenu() {
    auto imageProcessorsInfo = ImageProcessingInteractor::getImageProcessorsInfo();
    QMenu *comparatorsMenu = ui->menuComparators;
    QMenu *filtersMenu = ui->menuFilters;
    imageProcessorsMenuController->buildFiltersAndComparatorsMenus(comparatorsMenu,
                                                                   filtersMenu,
                                                                   imageProcessorsInfo
                                                                   );
}

void MainWindow::makeConnections() {
    connect(ui->actionOpenImages, &QAction::triggered, this, &MainWindow::openImages);
    connect(ui->actionCloseImages, &QAction::triggered, this, &MainWindow::closeImages);
    connect(ui->actionSwitchBetweenImages, &QAction::triggered, this, &MainWindow::switchBetweenImages);
    connect(ui->actionSaveVisibleAreaAs, &QAction::triggered, this, &MainWindow::saveVisibleAreaAs);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(ui->actionColorPicker, &QAction::triggered, this, &MainWindow::showColorPicker);
    connect(ui->actionShowOriginalImage, &QAction::triggered, this, &MainWindow::showOriginalImages);
    connect(ui->actionActualSize, &QAction::triggered, this, &MainWindow::imageZoomedToActualSize);
    connect(ui->actionFitInView, &QAction::triggered, this, &MainWindow::imagFitInView);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::imageZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::imageZoomOut);
    connect(ui->actionSaveImageAs, &QAction::triggered, this, &MainWindow::saveImageAs);
    connect(ui->actionPlaceColorPickerOnRight, &QAction::triggered, this, &MainWindow::placeColorPickerOnRight);
    connect(ui->actionPlaceColorPickerOnLeft, &QAction::triggered, this, &MainWindow::placeColorPickerOnLeft);
    connect(ui->actionGrabImagesFromVideos, &QAction::triggered, this, &MainWindow::grabImagesFromVideos);
    connect(ui->actionRunAllComparators, &QAction::triggered, this, &MainWindow::runAllComparators);
    connect(ui->actionPluginsSettings, &QAction::triggered, this, &MainWindow::changePluginsSettings);
    connect(ui->actionRescanPluginDir, &QAction::triggered, this, &MainWindow::rescanPluginDir);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::callImageProcessorsHelp);
}

void MainWindow::enableImageProceesorsMenuItems(bool isEnabled) {
    ui->menuComparators->setDisabled(!isEnabled);
    ui->menuFilters->setDisabled(!isEnabled);
    ui->menuImageAnalysis->setDisabled(!isEnabled);
    ui->actionSaveImageAs->setDisabled(!isEnabled);
    ui->actionSaveVisibleAreaAs->setDisabled(!isEnabled);
    ui->actionCloseImages->setDisabled(!isEnabled);
    ui->actionActualSize->setDisabled(!isEnabled);
    ui->actionZoomIn->setDisabled(!isEnabled);
    ui->actionZoomOut->setDisabled(!isEnabled);
    ui->actionShowOriginalImage->setDisabled(!isEnabled);
    ui->actionSwitchBetweenImages->setDisabled(!isEnabled);
    ui->actionPlaceColorPickerOnLeft->setDisabled(!isEnabled);
    ui->actionPlaceColorPickerOnRight->setDisabled(!isEnabled);
    ui->actionFitInView->setDisabled(!isEnabled);
    ui->actionAdvancedColorPicker->setDisabled(!isEnabled);
    ui->actionColorPicker->setDisabled(!isEnabled);
}

void MainWindow::updateRecentFilesMenu() {
    QStringList recentFileMenuRecords = recentFilesInteractor->getRecentFilesMenuRecords();

    ui->menuRecentFiles->clear();

    foreach (auto record, recentFileMenuRecords) {
        QAction *action = new QAction(record, ui->menuRecentFiles);
        action->setData(record);
        connect(action, &QAction::triggered, this, &MainWindow::openRecentFile);
        ui->menuRecentFiles->addAction(action);
    }

    if (ui->menuRecentFiles->isEmpty()) {
        ui->menuRecentFiles->addAction("No Recent Files")->setEnabled(false);
    } else {
        ui->menuRecentFiles->addAction("")->setSeparator(true);

        QAction *action = new QAction("Clear Menu", ui->menuRecentFiles);
        connect(action, &QAction::triggered, this, &MainWindow::clearOpenRecentsMenu);
        ui->menuRecentFiles->addAction(action);
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Event handlers for application menu interactions { */

void MainWindow::grabImagesFromVideos() {
    imageFilesInteractor->openImagesFromVideos();
}

void MainWindow::runAllComparators() {
    if (imageProcessingInteractor != nullptr) {
        imageProcessingInteractor->runAllComparators();
    }
}

void MainWindow::changePluginsSettings() {
    PluginsSettingsDialog dialog {};
    dialog.exec();
}

void MainWindow::rescanPluginDir() {
    buildImageProcessorsMenu();
    makeConnections();
    enableImageProceesorsMenuItems(imageView->hasActiveSession());
}

void MainWindow::clearOpenRecentsMenu() {
    recentFilesInteractor->clear();
    updateRecentFilesMenu();
}

void MainWindow::imageZoomedToActualSize() {
    imageView->setToActualSize();
}

void MainWindow::imagFitInView() {
    imageView->setToFitImageInView();
}

void MainWindow::imageZoomIn() {
    imageView->zoomIn();
}

void MainWindow::imageZoomOut() {
    imageView->zoomOut();
}

void MainWindow::openRecentFile() {
    QAction *action = qobject_cast<QAction*>(sender());
    if (action == nullptr) {
        return;
    }
    auto data = action->data();
    if (!data.isValid()) {
        return;
    }
    imageFilesInteractor->openImagesFromRecentMenu(data.toString());
}

void MainWindow::openImages() {
    imageFilesInteractor->openImagesViaOpenFilesDialog();
}

void MainWindow::closeImages() {
    onImagesClosed();
}

void MainWindow::switchBetweenImages() {
    imageView->toggleImage();
}

void MainWindow::callImageProcessor() {
    if (imageProcessingInteractor == nullptr) {
        return;
    }
    QAction *action = qobject_cast<QAction*>(sender());
    if (action == nullptr) {
        return;
    }
    auto data = action->data();
    if (!data.isValid()) {
        return;
    }
    imageProcessingInteractor->callImageProcessor(data.toString());
}

void MainWindow::callImageProcessorsHelp() {
    if (imageProcessingInteractor != nullptr) {
        imageProcessingInteractor->showImageProcessorsHelp();
    }
}

void MainWindow::saveImageAs() {
    SaveImageInfo info = imageView->getImageShowedOnTheScreen();
    imageFilesInteractor->saveImage(info);
}

void MainWindow::saveVisibleAreaAs() {
    SaveImageInfo info = imageView->getCurrentVisiableArea();
    imageFilesInteractor->saveImage(info);
}

void MainWindow::showAboutDialog() {
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::showOriginalImages() {
    if (imageProcessingInteractor != nullptr) {
        imageProcessingInteractor->restoreOriginalImages();
    }
}

void MainWindow::showColorPicker() {
    colorPickerController->openColorPickerDialog();
}

void MainWindow::placeColorPickerOnRight() {
    colorPickerController->placeColorPickerToRightSideOfMainWindow();
}

void MainWindow::placeColorPickerOnLeft() {
    colorPickerController->placeColorPickerToLeftSideOfMainWindow();
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Color Picker dialog { */

bool MainWindow::event(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        bool isMinimized = this->windowState() & Qt::WindowMinimized;
        colorPickerController->onMainWindowStateChanged(isMinimized);
    }
    return QMainWindow::event(event);
}

void MainWindow::onColorUnderCursorTrackingStatusChanged(bool isActive) {
    imageView->onColorUnderCursorTrackingStatusChanged(isActive);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Save and restore the main widow position { */

void MainWindow::saveMainWindowPosition() {
    QSettings settings("com.WhisperingWind", "ImageDiff");
    settings.setValue("mainWindow/geometry", saveGeometry());
    settings.setValue("mainWindow/state", saveState());
}

void MainWindow::restoreMainWindowPosition() {
    QSettings settings("com.WhisperingWind", "ImageDiff");
    restoreGeometry(settings.value("mainWindow/geometry").toByteArray());
    restoreState(settings.value("mainWindow/state").toByteArray());
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Drag and drop support to simplify file opening { */

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasUrls()) {
        event->ignore();
        return;
    }
    event->acceptProposedAction();
    QList<QUrl> urls = event->mimeData()->urls();
    onDrop(urls);
}

// IDropListener interface
void MainWindow::onDrop(QList<QUrl> urls) {
    try {
        imageFilesInteractor->openImagesFromDragAndDrop(urls);
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* MainWindow' close event { */

void MainWindow::closeEvent(QCloseEvent *) {
    saveMainWindowPosition();
    colorPickerController->onMainWindowClosed();
}
/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Methods of the abstract class IColorUnderCursorChangeListener { */

void MainWindow::onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                           const ImagePixelColor &hiddenImageRgbValue
                                           )
{
    colorPickerController->onColorUnderCursorChanged(visibleImageRgbValue, hiddenImageRgbValue);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Methods of the abstract class IImageFilesInteractorListener { */

void MainWindow::onImagesOpened(const ImagesPtr images) {
    if (imageProcessingInteractor != nullptr) {
        imageProcessingInteractor->unsubscribe(this);
        delete imageProcessingInteractor;
        imageProcessingInteractor = nullptr;

    }
    imageView->cleanUp();
    showStatusMessage("");

    imageProcessingInteractor = new ImageProcessingInteractor(images, this, this);
    imageProcessingInteractor->subscribe(this);
    imageView->displayImages(images);
    colorPickerController->onImagesOpened();
    enableImageProceesorsMenuItems(true);
    if (!images->getIsTemporaryFiles()) {
        recentFilesInteractor->addRecentFilesRecord(images->path1, images->path2);
    }
    updateRecentFilesMenu();
}

void MainWindow::onImagesOpenFailed(const QString &error) {
    QString errorMsg = "Unknow error!";
    if (!error.isEmpty()) {
        errorMsg = error;
    }
    showError(errorMsg);
}

void MainWindow::onImagesClosed() {
    if (imageProcessingInteractor != nullptr) {
        imageProcessingInteractor->unsubscribe(this);
        delete imageProcessingInteractor;
        imageProcessingInteractor = nullptr;

    }
    imageFilesInteractor->cleanup();
    colorPickerController->onImagesClosed();
    imageView->cleanUp();
    enableImageProceesorsMenuItems(false);
    showStatusMessage("");
}

void MainWindow::onSavingFileFailed(const QString &path) {
    showError("Error: Unable to open  " + path + ".");
}

void MainWindow::onFileSavedSuccessfully(const QString &path) {
    #ifdef QT_DEBUG
        if (!path.isEmpty()) {
            onMessage("File saved at path " + path + ".");
        }
    #endif
}

void MainWindow::onShowImageProcessorsHelp(const QString &html) {
    HelpDialog dialog(html);
    dialog.exec();
}

void MainWindow::onComparisonResultLoaded(const QPixmap &image, const QString &description) {
    imageView->showImageFromComparator(image, description);
}

void MainWindow::onComparisonResultLoaded(const QString &html,
                                          const QString &comparatorFullName,
                                          const QString &comparatorDescription,
                                          const QString &firstImagePath,
                                          const QString &secondImagePath
                                          )
{
    ComparatorResultDialog dialog { html,
                                    comparatorFullName,
                                    comparatorDescription,
                                    firstImagePath,
                                    secondImagePath
                                  };
    dialog.exec();
}

void MainWindow::onFilteredResultLoaded(const QPixmap &firstImage, const QPixmap &secondImage) {
    imageView->replaceDisplayedImages(firstImage, secondImage);
}

void MainWindow::onImageProcessorFailed(const QString &error) {
    QString errorMsg = "Unknow error!";
    if (!error.isEmpty()) {
        errorMsg = error;
    }
    showError(errorMsg);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Methods of the abstract class IPropcessorPropertiesDialogCallback { */

/*
 * Opens a dialog that displays all the properties of the image processor.
 * The user can change the values. The function returns the new properties
 * or the default ones if nothing was changed.
 */
QList<Property> MainWindow::showImageProcessorPropertiesDialog(const QString& processorName,
                                                               const QString& processorDescription,
                                                               const QList<Property>& defaultProperties
                                                               )
{
    PropertyEditorDialog dialog(processorName, processorDescription, defaultProperties, this);
    dialog.setWindowTitle("Properties");
    dialog.setModal(true);
    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
        return dialog.getUpdatedProperties();
    } else {
        /*
         * Not the best way to interrupt the image processor's execution,
         * but it's safe since exceptions are caught.
         * TBD: Implement proper interruption when the properties dialog
         * is closed, if the user clicks Cancel.
         */
        throw std::exception();
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Show messages { */

void MainWindow::showError(const QString &errorMessage) {
    qDebug() << errorMessage;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::showStatusMessage(QString message) {
    statusBar()->showMessage(message);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* If the user selects a part of the image while holding down Control,
 * we copy the selected part and open it in a new instance of the application
 * for further analysis. Parts from both images are copied.
 *  {   */

void MainWindow::openImagesInOtherAppInstance(QString firstFilePath,
                                              QString secondFilePath
                                             )
{
    // FIXME find the finished processes and remove these from the collection
    auto process = make_shared<QProcess>();
    instances.append(process);

    QString program = QCoreApplication::applicationFilePath();
    QStringList arguments;
    arguments << firstFilePath << secondFilePath;

    if (process->startDetached(program, arguments)) {
        showMinimized();
    } else {
        showError("Failed to start process: " + process->errorString());
    }
}

void MainWindow::openImagesFromCommandLine(const QString &firstFilePath, const QString &secondFilePath) {
    imageFilesInteractor->openImagesViaCommandLine(firstFilePath, secondFilePath);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* IProgressDialog interface {   */

void MainWindow::showProgressDialog(const QString &caption, int totalSteps) {
    if (progressDialog != nullptr) {
        delete progressDialog;
    }
    progressDialog = new QProgressDialog(this);
    progressDialog->setWindowTitle(caption);
    progressDialog->setRange(0, totalSteps);
    progressDialog->setValue(0);
    progressDialog->setCancelButtonText("Cancel");
    progressDialog->setMinimumDuration(0);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setAutoClose(false);
}

bool MainWindow::wasCanceled() {
    if (progressDialog == nullptr) {
        return true;
    }
    bool canceled = progressDialog->wasCanceled();
    if (canceled) {
        progressDialog->close();
        delete progressDialog;
        progressDialog = nullptr;
    }
    return canceled;
}

void MainWindow::onUpdateProgressValue(int value) {
    if (progressDialog == nullptr) {
        return;
    }
    progressDialog->setValue(value);
    QCoreApplication::processEvents();
    if (value >= progressDialog->maximum()) {
        progressDialog->close();
        delete progressDialog;
        progressDialog = nullptr;
    }
}

void MainWindow::onMessage(const QString &message) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::onError(const QString &error) {
    showError(error);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


