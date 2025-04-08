#include "mainwindow.h"

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
#include <QThread>
#include <QClipboard>
#include <presentation/colorpickercontroller.h>
#include <business/getimagesfromvideosinteractor.h>
#include <presentation/views/imageviewer.h>
#include <presentation/dialogs/aboutdialog.h>
#include <presentation/dialogs/comparatorresultdialog.h>
#include <presentation/dialogs/externalimageviewerdialog.h>
#include <presentation/dialogs/helpdialog.h>
#include <presentation/dialogs/imageautoanalysissettingsdialog.h>
#include <presentation/dialogs/pluginssettingsdialog.h>
#include <presentation/dialogs/propertyeditordialog.h>
#include <data/storage/filedialoghandler.h>
#include <business/imageanalysis/imageprocessinginteractor.h>
#include <domain/interfaces/presentation/iotherappinstancesinteractorcallback.h>
#include <business/otherappinstancesinteractor.h>
#include <business/recentfilesinteractor.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    mMenuIsInSingleImageMode(false)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    setWindowTitle("TwinPix");

    resize(1380, 820);
    showNormal();
    restoreMainWindowPosition();

    mColorPickerController = new ColorPickerController(this);
    mImageView = new ImageViewer(this, this);
    mImageFilesInteractor = new ImageFilesInteractor();
    mImageProcessorsMenuController = new ImageProcessorsMenuController(this);
    mRecentFilesInteractor = new RecentFilesInteractor();
    mOtherAppInstanceInteractor = new OtherAppInstancesInteractor(this);
    mImageProcessingInteractor = nullptr;
    mProgressDialog = nullptr;

    setCentralWidget(mImageView);

    mImageFilesInteractor->subscribe(this);

    buildImageProcessorsMenu();
    makeConnections();
    enableImageProceesorsMenuItems(false);

    updateRecentFilesMenu();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showEvent(QShowEvent *) {
    onFastSwitchingToComparisonImageStatusChanged(false);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {

    if (event->button() != Qt::RightButton) {
        return;
    }

    QMenu contextMenu(this);

    QMenu *filters = ui->menuFilters;
    if (filters) {
        contextMenu.addActions(filters->actions());
    }
    if (!mMenuIsInSingleImageMode) {
        contextMenu.addSeparator();
        QMenu *comparators = ui->menuComparators;
        if (comparators) {
            contextMenu.addActions(comparators->actions());
        }
    }
    contextMenu.exec(event->globalPosition().toPoint());
}

/*  Application menu settings { */

void MainWindow::buildImageProcessorsMenu() {
    auto imageProcessorsInfo = ImageProcessingInteractor::getImageProcessorsInfo();
    QMenu *comparatorsMenu = ui->menuComparators;
    QMenu *filtersMenu = ui->menuFilters;
    mImageProcessorsMenuController->buildFiltersAndComparatorsMenus(comparatorsMenu,
                                                                   filtersMenu,
                                                                   imageProcessorsInfo
                                                                   );
}

void MainWindow::makeConnections() {
    connect(ui->actionOpenImages, &QAction::triggered, this, &MainWindow::openImages);
    connect(ui->actionOpenImage, &QAction::triggered, this, &MainWindow::openImage);
    connect(ui->actionCloseImages, &QAction::triggered, this, &MainWindow::closeImages);
    connect(ui->actionSwitchBetweenImages, &QAction::triggered, this, &MainWindow::switchBetweenImages);
    connect(ui->actionSaveVisibleAreaAs, &QAction::triggered, this, &MainWindow::saveVisibleAreaAs);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(ui->actionColorPicker, &QAction::triggered, this, &MainWindow::showDockedColorPicker);
    connect(ui->actionShowOriginalImage, &QAction::triggered, this, &MainWindow::reloadImagesFromDisk);
    connect(ui->actionActualSize, &QAction::triggered, this, &MainWindow::imageZoomedToActualSize);
    connect(ui->actionFitInView, &QAction::triggered, this, &MainWindow::imagFitInView);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::imageZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::imageZoomOut);
    connect(ui->actionSaveImageAs, &QAction::triggered, this, &MainWindow::saveImageAs);
    connect(ui->actionPlaceColorPickerOnRight, &QAction::triggered, this, &MainWindow::placeColorPickerOnRight);
    connect(ui->actionPlaceColorPickerOnLeft, &QAction::triggered, this, &MainWindow::placeColorPickerOnLeft);
    connect(ui->actionGetImagesFromVideos, &QAction::triggered, this, &MainWindow::getImagesFromVideos);
    connect(ui->actionRunAllComparators, &QAction::triggered, this, &MainWindow::runAllComparators);
    connect(ui->actionPluginsSettings, &QAction::triggered, this, &MainWindow::changePluginsSettings);
    connect(ui->actionRescanPluginDir, &QAction::triggered, this, &MainWindow::rescanPluginDir);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::callImageProcessorsHelp);
    connect(ui->actionShowFirstImage, &QAction::triggered, this, &MainWindow::showFirstImage);
    connect(ui->actionShowSecondImage, &QAction::triggered, this, &MainWindow::showSecondImage);
    connect(ui->actionShowComparisonImage, &QAction::triggered, this, &MainWindow::showComparisonImage);
    connect(ui->actionImageAutoAnalysisSettings, &QAction::triggered, this, &MainWindow::showImageAutoAnalysisSettings);
    connect(ui->actionOpenImageFromClipboard, &QAction::triggered, this, &MainWindow::openImageFromClipboard);
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
    ui->actionColorPicker->setDisabled(!isEnabled);
    ui->actionShowFirstImage->setDisabled(!isEnabled);
    ui->actionShowSecondImage->setDisabled(!isEnabled);

    if (mMenuIsInSingleImageMode) {
        ui->menuComparators->setDisabled(true);
        ui->menuImageAnalysis->setDisabled(true);
        ui->actionSwitchBetweenImages->setDisabled(true);
        ui->actionShowFirstImage->setDisabled(true);
        ui->actionShowSecondImage->setDisabled(true);
        ui->actionShowComparisonImage->setDisabled(true);
    }
}

void MainWindow::updateRecentFilesMenu() {
    QStringList recentFileMenuRecords = mRecentFilesInteractor->getRecentFilesMenuRecords();

    ui->menuRecentImages->clear();

    foreach (auto record, recentFileMenuRecords) {
        QAction *action = new QAction(record, ui->menuRecentImages);
        action->setData(record);
        connect(action, &QAction::triggered, this, &MainWindow::openRecentFile);
        ui->menuRecentImages->addAction(action);
    }

    if (ui->menuRecentImages->isEmpty()) {
        ui->menuRecentImages->addAction("No Recent Files")->setEnabled(false);
    } else {
        ui->menuRecentImages->addAction("")->setSeparator(true);

        QAction *action = new QAction("Clear Menu", ui->menuRecentImages);
        connect(action, &QAction::triggered, this, &MainWindow::clearOpenRecentsMenu);
        ui->menuRecentImages->addAction(action);
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Event handlers for application menu interactions { */

void MainWindow::getImagesFromVideos() {
    mImageFilesInteractor->openImagesFromVideos();
}

void MainWindow::runAllComparators() {
    if (mImageProcessingInteractor != nullptr) {
        mImageProcessingInteractor->runAllComparators();
    }
}

void MainWindow::changePluginsSettings() {
    PluginsSettingsDialog dialog {};
    dialog.exec();
    rescanPluginDir();
}

void MainWindow::rescanPluginDir() {
    buildImageProcessorsMenu();
    enableImageProceesorsMenuItems(mImageView->hasActiveSession());
}

void MainWindow::clearOpenRecentsMenu() {
    mRecentFilesInteractor->clear();
    updateRecentFilesMenu();
}

void MainWindow::imageZoomedToActualSize() {
    mImageView->setToActualSize();
}

void MainWindow::imagFitInView() {
    mImageView->setToFitImageInView();
}

void MainWindow::imageZoomIn() {
    mImageView->zoomIn();
}

void MainWindow::imageZoomOut() {
    mImageView->zoomOut();
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
    mImageFilesInteractor->openImagesFromRecentMenu(data.toString());
}

void MainWindow::openImages() {
    mImageFilesInteractor->openImagesViaOpenFilesDialog();
}

void MainWindow::openImage() {
    mImageFilesInteractor->openImageViaOpenFilesDialog();
}

void MainWindow::closeImages() {
    onImagesClosed();
}

void MainWindow::switchBetweenImages() {
    mImageView->toggleImage();
}

void MainWindow::showFirstImage() {
    mImageView->showFirstImage();
}

void MainWindow::showSecondImage() {
    mImageView->showSecondImage();
}

void MainWindow::showComparisonImage() {
    mImageProcessingInteractor->showLastComparisonImage();
}

void MainWindow::showImageAutoAnalysisSettings() {
    ImageAutoAnalysisSettingsDialog dialog{};
    dialog.exec();
}

void MainWindow::openImageFromClipboard() {
    mImageFilesInteractor->openImageFromClipboard();
}

void MainWindow::callImageProcessor() {
    if (mImageProcessingInteractor == nullptr) {
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
    mImageProcessingInteractor->callImageProcessor(data.toString());
}

void MainWindow::callImageProcessorsHelp() {
    QList<ImageProcessorInfo> info = ImageProcessingInteractor::showImageProcessorsHelp();
    HelpDialog dialog(info);
    dialog.exec();
}

void MainWindow::saveImageAs() {
    SaveImageInfo info = mImageView->getImageShowedOnTheScreen();
    mImageFilesInteractor->saveImageAs(info);
}

void MainWindow::saveVisibleAreaAs() {
    SaveImageInfo info = mImageView->getCurrentVisiableArea();
    mImageFilesInteractor->saveImageAs(info);
}

void MainWindow::showAboutDialog() {
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::reloadImagesFromDisk() {
    if (mImageProcessingInteractor != nullptr) {
        mImageProcessingInteractor->restoreOriginalImages();
    }
}

void MainWindow::showDockedColorPicker() {
    mColorPickerController->openColorPickerDialog();
}

void MainWindow::placeColorPickerOnRight() {
    mColorPickerController->placeColorPickerToRightSideOfMainWindow();
}

void MainWindow::placeColorPickerOnLeft() {
    mColorPickerController->placeColorPickerToLeftSideOfMainWindow();
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Color Picker dialog { */

void MainWindow::onColorUnderCursorTrackingStatusChanged(bool isActive) {
    mImageView->onColorUnderCursorTrackingStatusChanged(isActive);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Save and restore the main widow position { */

void MainWindow::saveMainWindowPosition() {
    QSettings settings("com.WhisperingWind", "TwinPix");
    settings.setValue("mainWindow/geometry", saveGeometry());
    settings.setValue("mainWindow/state", saveState());
}

void MainWindow::restoreMainWindowPosition() {
    QSettings settings("com.WhisperingWind", "TwinPix");
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
        mImageFilesInteractor->openImagesFromDragAndDrop(urls);
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* If the user holds down the Command (Ctrl) key along with the comparator hotkey and selects
 * a specific area while holding the left mouse button, the comparator will run only for
 * the selected area. {   */

void MainWindow::onSelectedAreaShouldBeAnalyzed(ImageHolderPtr images, std::optional<int> key) {
    mImageProcessingInteractor->analyzeSelectedArea(images, key);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* If the user selects a part of the image while holding down Option (Alt),
 * we copy the selected part and open it in a new instance of the application
 * for further analysis. Parts from both images are copied.
 *  {   */

void MainWindow::onImagesCropped(ImageHolderPtr images) {
    mOtherAppInstanceInteractor->openNewAppInstance(images);
}

void MainWindow::onOtherAppInstanceOpened() {
    showMinimized();
}

void MainWindow::showError(const QString &errorMessage) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* MainWindow' close event { */

void MainWindow::closeEvent(QCloseEvent *) {
    saveMainWindowPosition();
}
/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Methods of the abstract class IColorUnderCursorChangeListener { */

void MainWindow::onColorUnderCursorChanged(const ImagePixelColor &visibleImageRgbValue,
                                           const std::optional<ImagePixelColor> &hiddenImageRgbValue
                                           )
{
    mColorPickerController->onColorUnderCursorChanged(visibleImageRgbValue, hiddenImageRgbValue);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Methods of the abstract class IImageFilesInteractorListener { */

void MainWindow::onImagesOpened(const ImageHolderPtr images) {
    if (mImageProcessingInteractor != nullptr) {
        mImageProcessingInteractor->unsubscribe(this);
        delete mImageProcessingInteractor;
        mImageProcessingInteractor = nullptr;

    }
    mMenuIsInSingleImageMode = images->isSingleImage();
    mImageView->cleanUp();
    mImageProcessingInteractor = new ImageProcessingInteractor(images, this, this);
    mImageProcessingInteractor->subscribe(this);
    mImageView->displayImages(images);
    mColorPickerController->onImagesOpened();
    enableImageProceesorsMenuItems(true);
    if (!images->isMarkedTemporary()) {
        if (images->isPairOfImages()) {
            mRecentFilesInteractor->addRecentFilesRecord(images->getFirstImagePath(),
                                                        images->getSecondImagePath()
                                                        );
        } else {
            mRecentFilesInteractor->addRecentFilesRecord(images->getFirstImagePath());
        }
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
    if (mImageProcessingInteractor != nullptr) {
        mImageProcessingInteractor->unsubscribe(this);
        delete mImageProcessingInteractor;
        mImageProcessingInteractor = nullptr;
    }
    mMenuIsInSingleImageMode = false;
    mImageFilesInteractor->cleanup();
    mColorPickerController->onImagesClosed();
    mImageView->cleanUp();
    enableImageProceesorsMenuItems(false);
    setWindowTitle("TwinPix");
}

void MainWindow::onSavingFileFailed(const QString &path) {
    showError("Unable to save at path  " + path + ".");
}

void MainWindow::onFileSavedSuccessfully(const QString &path) {
    #ifdef QT_DEBUG
        if (!path.isEmpty()) {
            onMessage("File saved at path " + path + ".");
        }
    #endif
}

void MainWindow::onComparisonResultLoaded(const QPixmap &image, const QString &description) {
    mImageView->showImageFromComparator(image, description);
}

void MainWindow::onComparisonResultLoaded(const QString &html,
                                          const QString &comparatorFullName,
                                          const QString &firstImagePath,
                                          const QString &secondImagePath
                                          )
{
    ComparatorResultDialog dialog { html,
                                    comparatorFullName,
                                    firstImagePath,
                                    secondImagePath
                                  };
    dialog.exec();
}

void MainWindow::onShowImageInExternalViewer(const QPixmap &image, const QString &description) {
    if (isMaximized()) {
        showMinimized();
    }

   // QMetaObject::invokeMethod(this, [=]() {
        ExternalImageViewerDialog dialog(image, description);
        dialog.exec();
    //}, Qt::QueuedConnection);
}

void MainWindow::onFilteredResultLoaded(const ImageHolderPtr imageHolder) {
    mImageView->replaceDisplayedImages(imageHolder);
}

void MainWindow::onImageProcessorFailed(const QString &error) {
    QString errorMsg = "Unknow error!";
    if (!error.isEmpty()) {
        errorMsg = error;
    }
    showError(errorMsg);
}
/*
 * The user can switch between images by pressing keys 1, 2, and 3,
 * where the number 3 means switching to the last available
 * comparison result image. However, there are cases when this
 * is not possible: the user has changed the original compared
 * images (applied or removed filters). In this case, the previous
 * comparison result is no longer valid, so the key 3 does not work.
 */
void MainWindow::onFastSwitchingToComparisonImageStatusChanged(bool isSwitchingAvailable) {
    ui->actionShowComparisonImage->setEnabled(isSwitchingAvailable);
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

/*  {   */

void MainWindow::onComparebleImageDisplayed(const QString &imageName) {
    QString title = QString("%1 - TwinPix").arg(imageName);
    setWindowTitle(title);
}

void MainWindow::onComparisonImageDisplayed(const QString &image1Name,
                                            const QString &image2Name,
                                            const QString &comparatorName) {
    QString title = QString("%1 vs %2 @ %3 - TwinPix").arg(image1Name, image2Name, comparatorName);
    setWindowTitle(title);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/*  {   */

void MainWindow::openImagesFromCommandLine(const QString &firstFilePath, const QString &secondFilePath) {
    mImageFilesInteractor->openImagesViaCommandLine(firstFilePath, secondFilePath);
}

void MainWindow::openImageFromCommandLine(const QString &filePath) {
    mImageFilesInteractor->openImageViaCommandLine(filePath);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* IProgressDialog interface {   */

void MainWindow::showProgressDialog(const QString &caption, int totalSteps) {
    if (mProgressDialog != nullptr) {
        delete mProgressDialog;
    }
    mProgressDialog = new QProgressDialog(this);
    mProgressDialog->setWindowTitle(caption);
    mProgressDialog->setRange(0, totalSteps);
    mProgressDialog->setValue(0);
    mProgressDialog->setCancelButtonText("Cancel");
    mProgressDialog->setMinimumDuration(0);
    mProgressDialog->setWindowModality(Qt::WindowModal);
    mProgressDialog->setAutoClose(false);
}

bool MainWindow::wasCanceled() {
    if (mProgressDialog == nullptr) {
        return true;
    }
    bool canceled = mProgressDialog->wasCanceled();
    if (canceled) {
        mProgressDialog->close();
        delete mProgressDialog;
        mProgressDialog = nullptr;
    }
    return canceled;
}

void MainWindow::onUpdateProgressValue(int value) {
    if (mProgressDialog == nullptr) {
        return;
    }
    mProgressDialog->setValue(value);
    QCoreApplication::processEvents();
    if (value >= mProgressDialog->maximum()) {
        mProgressDialog->close();
        delete mProgressDialog;
        mProgressDialog = nullptr;
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



