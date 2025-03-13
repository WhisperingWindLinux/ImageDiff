#include "gui/aboutdialog.h"
#include "gui/colorinfopanel.h"
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

#include <gui/imageviewstate.h>
#include <gui/propertyeditordialog.h>

#include <gui/formattors/recentfilespathformater.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    comparisionInteractor = new ComparisonInteractor(this);
    populateComparatorsAndFiltersMenus();

    connect(ui->actionOpenImages, &QAction::triggered, this, &MainWindow::openImages);
    connect(ui->actionCloseImages, &QAction::triggered, this, &MainWindow::closeImages);
    connect(ui->actionSwitchBetweenImages, &QAction::triggered, this, &MainWindow::switchBetweenImages);
    connect(ui->actionSaveVisibleAreaAs, &QAction::triggered, this, &MainWindow::saveVisibleAreaAs);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(ui->actionColorPicker, &QAction::triggered, this, &MainWindow::showColorPicker);
    connect(ui->actionShowOriginalImage, &QAction::triggered, this, &MainWindow::showOriginalImages);
    connect(ui->actionAdvancedColorPicker, &QAction::triggered, this, &MainWindow::showAdvancedColorPicker);
    connect(ui->actionActualSize, &QAction::triggered, this, &MainWindow::imageZoomedToActualSize);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::zoomOut);
    this->connect(ui->actionSaveImageAs, &QAction::triggered, this, &MainWindow::saveImageAs);


    setWindowTitle("Image Diff");
    resize(1380, 820);

    setAcceptDrops(true);
    showNormal();
}

MainWindow::~MainWindow() {
    delete ui;
    delete comparisionInteractor;
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
void MainWindow::populateComparatorsAndFiltersMenus() {
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
    disabledMenusIfImagesNotOpened();
}

void MainWindow::disabledMenusIfImagesNotOpened() {
    ui->menuComparators->setDisabled(true);
    ui->menuTransftomers->setDisabled(true);
    ui->menuImageAnalysis->setDisabled(true);
    ui->actionSaveImageAs->setDisabled(true);
    ui->actionSaveVisibleAreaAs->setDisabled(true);
    ui->actionCloseImages->setDisabled(true);
    ui->actionActualSize->setDisabled(true);
    ui->actionZoomIn->setDisabled(true);
    ui->actionZoomOut->setDisabled(true);
    ui->actionShowOriginalImage->setDisabled(true);
    ui->actionSwitchBetweenImages->setDisabled(true);
}

void MainWindow::enabledMenusIfImagesOpened() {
    ui->menuComparators->setDisabled(false);
    ui->menuTransftomers->setDisabled(false);
    ui->menuImageAnalysis->setDisabled(false);
    ui->actionSaveImageAs->setDisabled(false);
    ui->actionSaveVisibleAreaAs->setDisabled(false);
    ui->actionCloseImages->setDisabled(false);
    ui->actionActualSize->setDisabled(false);
    ui->actionZoomIn->setDisabled(false);
    ui->actionZoomOut->setDisabled(false);
    ui->actionShowOriginalImage->setDisabled(false);
    ui->actionSwitchBetweenImages->setDisabled(false);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Event handlers for application menu interactions { */


void MainWindow::imageZoomedToActualSize() {
    if (viewer != nullptr) {
        viewer->actualSize();
    }
}

void MainWindow::zoomIn() {
    if (viewer != nullptr) {
        viewer->zoomIn();
    }

}

void MainWindow::zoomOut() {
    if (viewer != nullptr) {
        viewer->zoomOut();
    }
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
        comparisionInteractor->loadImagesBeingCompared(recentMenuRecord);
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

void MainWindow::openImages() {
    closeImages();
    loadImagesBeingCompared();
}

void MainWindow::closeImages() {
    closeColorPickerDialog();
    disabledMenusIfImagesNotOpened();
    deleteImageView();
}

void MainWindow::switchBetweenImages() {
    if (viewer == nullptr) {
        return;
    }
    viewer->toggleImage();
}

void MainWindow::callImageComparator() {
    if (viewer == nullptr) {
        return;
    }
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
    if (viewer == nullptr) {
        return;
    }
    SaveImageInfo info = viewer->getImageShowedOnTheScreen();
    comparisionInteractor->saveImage(info);
}

void MainWindow::saveVisibleAreaAs() {
    if (viewer == nullptr) {
        return;
    }
    SaveImageInfo info = viewer->getCurrentVisiableArea();
    comparisionInteractor->saveImage(info);
}

void MainWindow::showAboutDialog() {
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::showOriginalImages() {
    if (viewer == nullptr) {
        return;
    }
    comparisionInteractor->realoadImagesFromDisk();
}

void MainWindow::showColorPicker() {
    openColorPickerDialog(true);
}

void MainWindow::showAdvancedColorPicker() {
    openColorPickerDialog(false);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Color Picker dialog { */

void MainWindow::openColorPickerDialog(bool isOnePanelMode) {
    if (viewer == nullptr) {
        return;
    }
    closeColorPickerDialog();
    colorPanel = new ColorInfoPanel(isOnePanelMode);
    colorPanel->show();
    viewer->onColorPickerStatusChanged(true);
}

void MainWindow::closeColorPickerDialog() {
    if (colorPanel != nullptr) {
        colorPanel->close();
        delete colorPanel;
        colorPanel = nullptr;
    }
    if (viewer != nullptr) {
        viewer->onColorPickerStatusChanged(false);
    }
}

/* } */

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
        comparisionInteractor->loadImagesBeingCompared(urls);
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* MainWindow' close event { */

void MainWindow::closeEvent(QCloseEvent *event) {
    closeColorPickerDialog();
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

void MainWindow::loadImagesBeingCompared() {
    try {
        QString firstImagePath = QFileDialog::getOpenFileName(nullptr, "Open First Image", "", "Images (*.png)");
        QString secondImagePath = QFileDialog::getOpenFileName(nullptr, "Open Second Image", "", "Images (*.png)");
        comparisionInteractor->loadImagesBeingCompared(firstImagePath, secondImagePath);
    } catch (std::runtime_error &e) {
        deleteImageView();
        showError(e.what());
    }
}

void MainWindow::onRgbValueUnderCursonChanged(RgbValue visibleImageRgbValue, RgbValue hiddenImageRgbValue) {
    if (colorPanel == nullptr || viewer == nullptr) {
        return;
    }
    if (colorPanel->isVisible()) {
        colorPanel->updateBothPanelsAndHighlightDifferences(visibleImageRgbValue, hiddenImageRgbValue);
    }
}

void MainWindow::onImagesBeingComparedLoadedSuccessfully(QPixmap& image1,
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
    closeImages();
    createImageView();
    viewer->showImagesBeingCompared(image1, path1, image2, path2, imageViewState);
    enabledMenusIfImagesOpened();
}

void MainWindow::onComparisonImagesLoaded(QPixmap &image, QString description) {
    if (viewer != nullptr) {
        viewer->showComparisonImage(image, description);
    }
}

void MainWindow::onComparisonTextLoaded(QString text) {
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
QList<Property> MainWindow::getUpdatedPropertiesFromUser(QList<Property> defaultProperties) {
    PropertyEditorDialog dialog(defaultProperties, this);
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
        throw new std::exception();
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
}

void MainWindow::createImageView() {
    if (viewer != nullptr) {
        viewer->close();
        setCentralWidget(nullptr);
        delete viewer;
    }
    viewer = new ImageViewer(this);
    setCentralWidget(viewer);
}

void MainWindow::showStatusMessage(QString message) {
    statusBar()->showMessage(message);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */







