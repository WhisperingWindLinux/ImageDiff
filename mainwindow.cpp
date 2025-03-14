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
    connect(ui->actionSaveImageAs, &QAction::triggered, this, &MainWindow::saveImageAs);
    connect(ui->actionPlaceColorPickerOnRight, &QAction::triggered, this, &MainWindow::placeColorPickerOnRight);
    connect(ui->actionPlaceColorPickerOnLeft, &QAction::triggered, this, &MainWindow::placeColorPickerOnLeft);

    setWindowTitle("Image Diff");
    resize(1380, 820);

    setAcceptDrops(true);
    showNormal();

    restoreMainWindowPosition();
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
    enabledImageOperationMenuItems(false);
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
        comparisionInteractor->loadTwoImagesBeingCompared(recentMenuRecord);
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

void MainWindow::openImages() {
    closeImages();
    loadTwoImagesBeingCompared();
}

void MainWindow::closeImages() {
    closeColorPickerDialog();
    enabledImageOperationMenuItems(false);
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

void MainWindow::placeColorPickerOnRight() {
    if (colorPanel != nullptr) {
        positionColorPickerWindow(colorPanel, colorPickerAlignmentPercent, true);
    } else {
        openColorPickerDialog(false);
        positionColorPickerWindow(colorPanel, colorPickerAlignmentPercent, true);
    }
}

void MainWindow::placeColorPickerOnLeft() {
    if (colorPanel != nullptr) {
        positionColorPickerWindow(colorPanel, colorPickerAlignmentPercent, false);
    } else {
        openColorPickerDialog(false);
        positionColorPickerWindow(colorPanel, colorPickerAlignmentPercent, true);
    }
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
    placeColorPickerOnRight();
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

bool MainWindow::event(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (this->windowState() & Qt::WindowMinimized) {
            colorPanel->hide();
        } else {
            colorPanel->show();
        }
    }
    return QMainWindow::event(event);
}

// Calculates the position of the color picker window relative to the main window (QMainWindow).
// Checks if the color picker window fits on the screen.
// If necessary, shifts the main window to the left or adjusts its size. The position of the second
// window vertically is set as a percentage (alignmentPercent) of the height of the main window.
// A value of 50% places the second window centered relative to the vertical axis of the main window.
void MainWindow::positionColorPickerWindow(QWidget *colorPickerWindow, int alignmentPercent, bool placeOnRight) {
    // Ensure alignmentPercent is within valid range
    alignmentPercent = qBound(0, alignmentPercent, 100);

    // Get screen geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry(); // Excludes taskbar/Dock area

    // Get main window geometry
    QRect mainGeometry = geometry();

    // Calculate target position for the second window
    int secondWindowX;
    if (placeOnRight) {
        // Place the color picker to the right of the main window
        secondWindowX = mainGeometry.right() + 1; // Start just to the right of the main window
    } else {
        // Place the color picker to the left of the main window
        secondWindowX = mainGeometry.left() - colorPickerWindow->width() - 1; // Start just to the left of the main window
    }

    int secondWindowY = mainGeometry.y() +
                        (mainGeometry.height() - colorPickerWindow->height()) * (100 - alignmentPercent) / 100;

    // Adjust position if placing on the right and the second window goes off-screen horizontally
    if (placeOnRight && secondWindowX + colorPickerWindow->width() > screenGeometry.right()) {
        // Second window goes off-screen; adjust the main window
        int requiredShift = (secondWindowX + colorPickerWindow->width()) - screenGeometry.right();
        if (mainGeometry.x() - requiredShift >= screenGeometry.left()) {
            // Move the main window to the left without resizing
            mainGeometry.moveLeft(mainGeometry.x() - requiredShift);
        } else {
            // Move the main window as far left as possible and resize it
            int maxShift = mainGeometry.x() - screenGeometry.left();
            mainGeometry.moveLeft(screenGeometry.left());
            mainGeometry.setWidth(mainGeometry.width() - (requiredShift - maxShift));
        }
        setGeometry(mainGeometry);
        // Recalculate the second window's position after adjusting the main window
        secondWindowX = mainGeometry.right() + 1;
    }

    // Adjust position if placing on the left and the second window goes off-screen horizontally
    if (!placeOnRight && secondWindowX < screenGeometry.left()) {
        // Second window goes off-screen; adjust the main window
        int requiredShift = screenGeometry.left() - secondWindowX;
        if (mainGeometry.right() + requiredShift <= screenGeometry.right()) {
            // Move the main window to the right without resizing
            mainGeometry.moveLeft(mainGeometry.x() + requiredShift);
        } else {
            // Move the main window as far right as possible and resize it
            int maxShift = screenGeometry.right() - mainGeometry.right();
            mainGeometry.moveLeft(mainGeometry.x() + maxShift);
            mainGeometry.setWidth(mainGeometry.width() - (requiredShift - maxShift));
        }
        setGeometry(mainGeometry);
        // Recalculate the second window's position after adjusting the main window
        secondWindowX = mainGeometry.left() - colorPickerWindow->width() - 1;
    }

    // Ensure the second window stays within vertical bounds of the screen
    if (secondWindowY < screenGeometry.top()) {
        secondWindowY = screenGeometry.top();
    } else if (secondWindowY + colorPickerWindow->height() > screenGeometry.bottom()) {
        secondWindowY = screenGeometry.bottom() - colorPickerWindow->height();
    }

    // Set the final position of the second window
    colorPickerWindow->move(secondWindowX, secondWindowY);
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

void MainWindow::loadTwoImagesBeingCompared() {
    try {
        QString firstImagePath = QFileDialog::getOpenFileName(nullptr, "Open First Image", "", "Images (*.png)");
        QString secondImagePath = QFileDialog::getOpenFileName(nullptr, "Open Second Image", "", "Images (*.png)");
        comparisionInteractor->loadTwoImagesBeingCompared(firstImagePath, secondImagePath);
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
    closeImages();
    createImageView();
    viewer->showTwoImagesBeingCompared(image1, path1, image2, path2, imageViewState);
    enabledImageOperationMenuItems(true);
}

void MainWindow::onImageResultFromComparatorReceived(QPixmap &image, QString description) {
    if (viewer != nullptr) {
        viewer->showImageFromComparator(image, description);
    }
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
