#include "gui/aboutdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/imageviewer.h"
#include "imageprocessorsmanager/imageprocessorsmanager.h"
#include "interactors/savefileinfo.h"

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
#include <QFileDialog>
#include <QPixmap>
#include <QScrollBar>
#include <QDebug>
#include <qmessagebox.h>
#include <QDockWidget>
#include <QMimeData>
#include <qprocess.h>
#include <gui/imageviewstate.h>
#include <gui/propertyeditordialog.h>
#include <gui/formattors/recentfilespathformater.h>
#include <interactors/getimagesfromvideosinteractor.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    comparisionInteractor = new ComparisonInteractor(this);
    rgbTrackingInteractor = new RgbTrackingInteractor(this);

    buildMenu();
    makeConnections();

    setWindowTitle("Image Diff");
    resize(1380, 820);

    setAcceptDrops(true);

    showNormal();
    restoreMainWindowPosition();
}

MainWindow::~MainWindow() {
    delete ui;
}

/*  Application menu settings { */

// Populate the recent files menu when the main window is displayed on the screen.
void MainWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    updateRecentFilesMenu();
}

// The application can run comparators and filters - custom image processors classes
// that can be easily added to the application. For more information see the constructor'
// imageprocessormanager/imageprocessormanager.cpp
void MainWindow::buildMenu() {
    ImageProcessorsManager *manager = ImageProcessorsManager::instance();
    auto comporatorsInfo = manager->allProcessorsInfo();

    QMenu *comparatorsMenu = ui->menuComparators;
    QMenu *transformersMenu = ui->menuTransftomers;

    // an image comporators
    for (auto it = comporatorsInfo.begin(); it != comporatorsInfo.end(); ++it) {
        QString name = (*it).name;
        QString hotkey = (*it).hotkey;
        ImageProcessorType type = (*it).type;
        QAction *newAction;

        if (type == ImageProcessorType::Comparator) {
            newAction = comparatorsMenu->addAction(name);
        } else if (type == ImageProcessorType::Filter) {
            newAction = transformersMenu->addAction(name);
        } else {
            continue;
        }
        newAction->setData(name);
        newAction->setShortcut(QKeySequence(hotkey));
        connect(newAction, &QAction::triggered, this, &MainWindow::callImageComparator);
    }
    enabledImageOperationMenuItems(false);
}

void MainWindow::makeConnections() {
    connect(ui->actionOpenImages, &QAction::triggered, this, &MainWindow::openImages);
    connect(ui->actionCloseImages, &QAction::triggered, this, &MainWindow::closeImages);
    connect(ui->actionSwitchBetweenImages, &QAction::triggered, this, &MainWindow::switchBetweenImages);
    connect(ui->actionSaveVisibleAreaAs, &QAction::triggered, this, &MainWindow::saveVisibleAreaAs);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(ui->actionColorPicker, &QAction::triggered, this, &MainWindow::showColorPicker);
    connect(ui->actionShowOriginalImage, &QAction::triggered, this, &MainWindow::showOriginalImages);
    connect(ui->actionAdvancedColorPicker, &QAction::triggered, this, &MainWindow::showAdvancedColorPicker);
    connect(ui->actionActualSize, &QAction::triggered, this, &MainWindow::imageZoomedToActualSize);
    connect(ui->actionFitInView, &QAction::triggered, this, &MainWindow::imagFitInView);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::imageZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::imageZoomOut);
    connect(ui->actionSaveImageAs, &QAction::triggered, this, &MainWindow::saveImageAs);
    connect(ui->actionPlaceColorPickerOnRight, &QAction::triggered, this, &MainWindow::placeColorPickerOnRight);
    connect(ui->actionPlaceColorPickerOnLeft, &QAction::triggered, this, &MainWindow::placeColorPickerOnLeft);
    connect(ui->actionGrabImagesFromVideos, &QAction::triggered, this, &MainWindow::grabImagesFromVideos);
}

void MainWindow::enabledImageOperationMenuItems(bool isEnabled) {
    ui->menuComparators->setDisabled(!isEnabled);
    ui->menuTransftomers->setDisabled(!isEnabled);
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
        comparisionInteractor->loadTwoImagesBeingCompared(imagesPath->first,
                                                          imagesPath->second,
                                                          false,
                                                          false,
                                                          true
                                                          );
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

void MainWindow::imageZoomedToActualSize() {
    viewer->actualSize();
}

void MainWindow::imagFitInView() {
    viewer->fitImageInView();
}

void MainWindow::imageZoomIn() {
    viewer->zoomIn();
}

void MainWindow::imageZoomOut() {
    viewer->zoomOut();
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
        comparisionInteractor->loadTwoImagesBeingCompared(recentMenuRecord, true);
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

void MainWindow::openImages() {
    deleteImageView();
    loadTwoImagesBeingCompared();
}

void MainWindow::closeImages() {
    deleteImageView();
}

void MainWindow::switchBetweenImages() {
    viewer->toggleImage();
}

void MainWindow::callImageComparator() {
    QAction *action = qobject_cast<QAction*>(sender());
    try {
        comparisionInteractor->onImageProcessorShouldBeCalled(action->data());
    } catch (std::runtime_error &e) {
        showError(e.what());
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}

void MainWindow::saveImageAs() {
    SaveImageInfo info = viewer->getImageShowedOnTheScreen();
    comparisionInteractor->saveImage(info);
}

void MainWindow::saveVisibleAreaAs() {
    SaveImageInfo info = viewer->getCurrentVisiableArea();
    comparisionInteractor->saveImage(info);
}

void MainWindow::showAboutDialog() {
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::showOriginalImages() {
    comparisionInteractor->realoadImagesFromDisk();
}

void MainWindow::showColorPicker() {
    rgbTrackingInteractor->showColorPicker();
}

void MainWindow::showAdvancedColorPicker() {
    rgbTrackingInteractor->showAdvancedColorPicker();
}

void MainWindow::placeColorPickerOnRight() {
    rgbTrackingInteractor->placeColorPickerOnRight();
}

void MainWindow::placeColorPickerOnLeft() {
    rgbTrackingInteractor->placeColorPickerOnLeft();
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Color Picker dialog { */

bool MainWindow::event(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        bool isMinimized = this->windowState() & Qt::WindowMinimized;
        rgbTrackingInteractor->onMainWindowStateChanged(isMinimized);
    }
    return QMainWindow::event(event);
}

void MainWindow::onRgbTrackingStatusChanged(bool isActive) {
    if (viewer != nullptr) {
        viewer->onColorPickerStatusChanged(isActive);
    }
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
        comparisionInteractor->loadTwoImagesBeingCompared(urls);
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* MainWindow' close event { */

void MainWindow::closeEvent(QCloseEvent *event) {
    saveMainWindowPosition();
    rgbTrackingInteractor->onMainWindowClosed();
}
/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Methods of the abstract class AMainWindowCallbacks { */

// These methods are called from ComparisonInteractor

void MainWindow::saveImage(QPixmap &image, QString defaultPath) {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Save Image",                  // Dialog title
        defaultPath,                   // Default directory
        "Images (*.png)"               // File filters
        );

    if (filePath.isEmpty()) {
        return;
    }

    if (!image.save(filePath)) {
        QMessageBox::warning(this, "Error", "Failed to save the image.");
    }
}

void MainWindow::loadTwoImagesBeingCompared() {
    try {
        QString firstImagePath = QFileDialog::getOpenFileName(nullptr, "Open First Image", "", "Images (*.png)");
        QString secondImagePath = QFileDialog::getOpenFileName(nullptr, "Open Second Image", "", "Images (*.png)");
        comparisionInteractor->loadTwoImagesBeingCompared(firstImagePath, secondImagePath, false, false, true);
    } catch (std::runtime_error &e) {
        deleteImageView();
        showError(e.what());
    }
}

void MainWindow::onRgbValueUnderCursonChanged(RgbValue visibleImageRgbValue, RgbValue hiddenImageRgbValue) {
    rgbTrackingInteractor->onRgbValueUnderCursonChanged(visibleImageRgbValue, hiddenImageRgbValue);
}

void MainWindow::onTwoImagesBeingComparedLoadedSuccessfully(QPixmap& image1,
                                                         QString path1,
                                                         QPixmap& image2,
                                                         QString path2,
                                                         bool useSavedImageViewState
                                                         )
{
    std::shared_ptr<ImageViewState> imageViewState = nullptr;
    if (useSavedImageViewState && viewer) {
        imageViewState = std::make_shared<ImageViewState>(viewer->getCurrentState());
    }
    deleteImageView();
    createImageView();
    viewer->showTwoImagesBeingCompared(image1, path1, image2, path2, imageViewState);
}

void MainWindow::onImageResultFromComparatorReceived(QPixmap &image, QString description) {
    viewer->showImageFromComparator(image, description);
}

void MainWindow::onTextResultFromComparatorReceived(QString text) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Pixel Difference Analysis");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

/*
 * Opens a dialog that displays all the properties of the image processor.
 * The user can change the values. The function returns the new properties
 * or the default ones if nothing was changed.
 */
QList<Property> MainWindow::getUpdatedPropertiesFromUser(QString processorName,
                                                         QString processorDescription,
                                                         QList<Property> defaultProperties
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
    QStringList pairsfilesPath = comparisionInteractor->getRecentFiles();

    ui->menuRecentFiles->clear(); // Clear existing actions

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

/* Show error dialog { */

void MainWindow::showError(const QString &errorMessage) {
    qDebug() << errorMessage;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Add or remove the ImageView instance to/form MainWindow  { */

void MainWindow::deleteImageView() {
    if (viewer != nullptr) {
        viewer->close();
        setCentralWidget(nullptr);
        delete viewer;
        viewer = nullptr;
    }
    showStatusMessage("");
    enabledImageOperationMenuItems(false);
    rgbTrackingInteractor->onImageViewDestroyed();
}

void MainWindow::createImageView() {
    viewer = new ImageViewer(this);
    setCentralWidget(viewer);
    enabledImageOperationMenuItems(true);
}

void MainWindow::showStatusMessage(QString message) {
    statusBar()->showMessage(message);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* If the user selects a part of the image while holding down Control,
 * we copy the selected part and open it in a new instance of the application
 * for further analysis. Parts from both images are copied.
 *  {   */

void MainWindow::passTwoImagesBeingComparedToOtherAppInstance(QString firstFilePath,
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

void MainWindow::onReceiveTwoImagesBeingComparedViaCommandline(QString firstFilePath,
                                                               QString secondFilePath
                                                               )
{
    comparisionInteractor->loadTwoImagesBeingCompared(firstFilePath, secondFilePath, false, true, false);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */



