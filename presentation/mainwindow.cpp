#include "mainwindow.h"
#include "data/storage/repositories/imagesrepository.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Image Diff");
    resize(1380, 820);
    showNormal();
    restoreMainWindowPosition();

    setAcceptDrops(true);

    comparisionInteractor = new ComparisonInteractor(this);
    colorPickerController = new ColorPickerController(this);
    imageProcessorsMenuController = new ImageProcessorsMenuController(this);
    imageView = new ImageViewer(this);
    setCentralWidget(imageView);

    buildImageProcessorsMenu();
    makeConnections();
    enableImageProceesorsMenuItems(false);

    coreUpdateRecentFilesMenu();
}

MainWindow::~MainWindow() {
    delete ui;
}

/*  Application menu settings { */

void MainWindow::buildImageProcessorsMenu() {
    auto imageProcessorsInfo = comparisionInteractor->getImageProcessorsInfo();
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

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Event handlers for application menu interactions { */

void MainWindow::grabImagesFromVideos() {
    try {
        GetImagesFromVideosInteractor grabImagesFromVideosInteractor{};
        auto imagesPath = grabImagesFromVideosInteractor.grabImagesFromVideos();
        if (!imagesPath) {
            return;
        }
        comparisionInteractor->openImages(imagesPath->first,
                                                          imagesPath->second,
                                                          false,
                                                          true
                                                          );
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

void MainWindow::runAllComparators() {
    comparisionInteractor->runAllComparators();
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
    QString recentMenuRecord = data.toString();
    if (recentMenuRecord.isEmpty()) {
        return;
    }
    try {
        comparisionInteractor->openImagesFromRecentMenu(recentMenuRecord, true);
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

void MainWindow::openImages() {
    try {
        comparisionInteractor->getPathsFromUserAndOpenImages();
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

void MainWindow::closeImages() {
    colorPickerController->onImagesClosed();
    imageView->cleanUp();
    enableImageProceesorsMenuItems(false);
}

void MainWindow::switchBetweenImages() {
    imageView->toggleImage();
}

void MainWindow::callImageProcessor() {
    QAction *action = qobject_cast<QAction*>(sender());
    try {
        comparisionInteractor->callImageProcessor(action->data());
    } catch (std::runtime_error &e) {
        showError(e.what());
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}

void MainWindow::callImageProcessorsHelp() {
    comparisionInteractor->showImageProcessorsHelp();
}

void MainWindow::saveImageAs() {
    SaveImageInfo info = imageView->getImageShowedOnTheScreen();
    comparisionInteractor->saveImage(info);
}

void MainWindow::saveVisibleAreaAs() {
    SaveImageInfo info = imageView->getCurrentVisiableArea();
    comparisionInteractor->saveImage(info);
}

void MainWindow::showAboutDialog() {
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::showOriginalImages() {
    comparisionInteractor->showOriginalImages();
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
    try {
        event->acceptProposedAction();
        QList<QUrl> urls = event->mimeData()->urls();
        comparisionInteractor->openImagesFromDragAndDrop(urls);
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

/* Methods of the abstract class AMainWindowCallbacks { */

// These methods are called from ComparisonInteractor

void MainWindow::onColorUnderCursorChanged(ImagePixelColor visibleImageRgbValue,
                                           ImagePixelColor hiddenImageRgbValue
                                           )
{
    colorPickerController->onColorUnderCursorChanged(visibleImageRgbValue, hiddenImageRgbValue);
}

void MainWindow::displayImages(QPixmapPtr image1,
                               const QString& path1,
                               QPixmapPtr image2,
                               const QString& path2
                               )
{
    imageView->displayImages(image1, path1, image2, path2);
    colorPickerController->onImagesOpened();
    enableImageProceesorsMenuItems(true);
}

void MainWindow::onImageResultFromComparatorReceived(const QPixmapPtr image,
                                                     const QString& description
                                                     )
{
    imageView->showImageFromComparator(image, description);
}

void MainWindow::onTextResultFromComparatorReceived(const QString& message,
                                                    const QString& comparatorFullName,
                                                    const QString& comparatorDescription,
                                                    const QString& firstImageFilePath,
                                                    const QString& secondImageFilePath
                                                    )
{
    ComparatorResultDialog dialog { message,
                                    comparatorFullName,
                                    comparatorDescription,
                                    firstImageFilePath,
                                    secondImageFilePath
                                  };
    dialog.exec();
}

void MainWindow::saveImage(const QPixmapPtr image, const QString& defaultPath) {

    SaveFileDialogHandler fileDialogHandler;
    auto savedFilePath = fileDialogHandler.getUserSaveImagePath(defaultPath);
    if (!savedFilePath) {
        return;
    }
    if (!image->save(savedFilePath.value())) {
        QMessageBox::warning(this, "Error", "Failed to save the image.");
    }
}

void MainWindow::showHelp(const QString& message) {
    if (!message.isEmpty()) {
        HelpDialog dialog(message);
        dialog.exec();
    }
}

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

void MainWindow::updateRecentFilesMenu() {
    coreUpdateRecentFilesMenu();
}

void MainWindow::onDisplayedImagesShouldBeReplaced(const QPixmapPtr first, const QPixmapPtr second) {
    imageView->replaceDisplayedImages(first, second);
}

void MainWindow::coreUpdateRecentFilesMenu() {
    QStringList pairsfilesPath = comparisionInteractor->getRecentFiles();

    ui->menuRecentFiles->clear();

    foreach (auto pair, pairsfilesPath) {
        QAction *action = new QAction(pair, this);
        action->setData(pair);
        connect(action, &QAction::triggered, this, &MainWindow::openRecentFile);
        ui->menuRecentFiles->addAction(action);
    }

    if (ui->menuRecentFiles->isEmpty()) {
        ui->menuRecentFiles->addAction("No Recent Files")->setEnabled(false);
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

void MainWindow::openImagesFromCommandLine(QString firstFilePath, QString secondFilePath)
{
    comparisionInteractor->openImages(firstFilePath, secondFilePath, true, false);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* IProgressDialog interface {   */

void MainWindow::showProgressDialog(QString caption, int totalSteps) {
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

void MainWindow::onMessage(QString message) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::onError(QString error) {
    showError(error);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


