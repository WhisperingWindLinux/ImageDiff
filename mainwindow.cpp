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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    comparisionInteractor = new ComparisonInteractor(this);
    buildMenus();

    ui->menuComparators->setDisabled(true);
    ui->menuTransftomers->setDisabled(true);
    ui->menuImageAnalysis->setDisabled(true);

    connect(ui->actionOpenImages, &QAction::triggered, this, &MainWindow::actionOpenImages_triggered);
    connect(ui->actionCloseImages, &QAction::triggered, this, &MainWindow::actionCloseImages_triggered);
    connect(ui->actionSwitchBetweenImages, &QAction::triggered, this, &MainWindow::actionSwitchBetweenImages_triggered);
    connect(ui->actionSaveImageAs, &QAction::triggered, this, &MainWindow::actionSaveImageAs_triggered);
    connect(ui->actionSaveVisibleAreaAs, &QAction::triggered, this, &MainWindow::actionSaveVisibleAreaAs_triggered);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::actionAbout_triggered);
    connect(ui->actionColorPicker, &QAction::triggered, this, &MainWindow::actionColorPicker_triggered);
    connect(ui->actionShowOriginalImage, &QAction::triggered, this, &MainWindow::actionShowOriginalImage_triggered);
    connect(ui->actionAdvancedColorPicker, &QAction::triggered, this, &MainWindow::actionShowAdvancedColorPicker_triggered);

    setWindowTitle("Image Diff");
    resize(1380, 820);

    setAcceptDrops(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::buildMenus() {
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
        } else if (type == ImageProcessorType::Transformer) {
            newAction = transformersMenu->addAction(name);
        } else {
            continue;
        }
        newAction->setData(name);
        newAction->setShortcut(QKeySequence(hotkey));

        connect(newAction, &QAction::triggered, this, &MainWindow::actionImageComparatorsMenuItem_triggered);
    }
}

void MainWindow::showStatusMessage(QString message) {
    statusBar()->showMessage(message);
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

void MainWindow::openColorPickerDialog(bool isForVisibleImageOnly) {
    if (viewer == nullptr) {
        return;
    }
    if (colorPanel != nullptr) {
        colorPanel->close();
        delete colorPanel;
        colorPanel = nullptr;
    }
    colorPanel = new ColorInfoPanel(nullptr, isForVisibleImageOnly);
    colorPanel->show();
    viewer->onColorPickerStatusChanged(true);
}


void MainWindow::closeEvent(QCloseEvent *event) { closeColorPickerDialog(); }

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
        ui->menuComparators->setDisabled(false);
        ui->menuTransftomers->setDisabled(false);
        ui->menuImageAnalysis->setDisabled(false);
    } catch (std::runtime_error &e) {
        showError(e.what());
    }
}

void MainWindow::actionOpenImages_triggered() {
    actionCloseImages_triggered();

    bool isLoaded = loadImagesBeingCompared();

    if (isLoaded) {
        ui->menuComparators->setDisabled(false);
        ui->menuTransftomers->setDisabled(false);
        ui->menuImageAnalysis->setDisabled(false);
    }

    if (!isLoaded && viewer != nullptr) {
        setCentralWidget(nullptr);
        delete viewer;
        viewer = nullptr;
    }
}

void MainWindow::actionCloseImages_triggered() {
    if (viewer != nullptr) {
        setCentralWidget(nullptr);
        delete viewer;
        viewer = nullptr;
    }
    closeColorPickerDialog();
    showStatusMessage("");

    ui->menuComparators->setDisabled(true);
    ui->menuTransftomers->setDisabled(true);
    ui->menuImageAnalysis->setDisabled(true);
}

void MainWindow::actionSwitchBetweenImages_triggered() {
    if (viewer == nullptr) {
        return;
    }
    viewer->toggleImage();
}

void MainWindow::actionImageComparatorsMenuItem_triggered() {
    QAction *action = qobject_cast<QAction*>(sender());
    try {
        comparisionInteractor->onImageProcessorShouldBeCalled(action->data());
    } catch (std::runtime_error &e) {
        showError(e.what());
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}

void MainWindow::actionSaveImageAs_triggered() {
    if (viewer == nullptr) {
        return;
    }
    SaveImageInfo info = viewer->getImageShowedOnTheScreen();
    comparisionInteractor->saveImage(info);
}

void MainWindow::actionSaveVisibleAreaAs_triggered() {
    if (viewer == nullptr) {
        return;
    }
    SaveImageInfo info = viewer->getCurrentVisiableArea();
    comparisionInteractor->saveImage(info);
}

void MainWindow::actionAbout_triggered() {
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::actionShowOriginalImage_triggered() {
    comparisionInteractor->realoadImagesFromDisk();
}

void MainWindow::actionColorPicker_triggered() {
    openColorPickerDialog(true);
}

void MainWindow::actionShowAdvancedColorPicker_triggered() {
    openColorPickerDialog(false);
}

void MainWindow::saveImageAs(QPixmap &image, QString defaultPath) {
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

bool MainWindow::loadImagesBeingCompared() {
    try {
        QString firstImagePath = QFileDialog::getOpenFileName(nullptr, "Open First Image", "", "Images (*.png)");
        QString secondImagePath = QFileDialog::getOpenFileName(nullptr, "Open Second Image", "", "Images (*.png)");
        comparisionInteractor->loadImagesBeingCompared(firstImagePath, secondImagePath);
        return true;
    } catch (std::runtime_error &e) {
        showError(e.what());
        return false;
    }
}

void MainWindow::showError(const QString &errorMessage) {
    qDebug() << errorMessage;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::onRgbValueUnderCursonChanged(RgbValue visibleImageRgbValue, RgbValue hiddenImageRgbValue) {
    if (colorPanel == nullptr || viewer == nullptr) {
        return;
    }
    if (colorPanel->isVisible()) {
        colorPanel->updateBothPanelsAndHighlightDifferences(visibleImageRgbValue, hiddenImageRgbValue);
    }
}

// AMainWindowCallbacks interface

void MainWindow::onImagesBeingComparedLoaded(QPixmap& image1,
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
    if (viewer != nullptr) {
        setCentralWidget(nullptr);
        delete viewer;
        viewer = nullptr;
    }
    viewer = new ImageViewer(this);
    setCentralWidget(viewer);
    viewer->showImagesBeingCompared(image1, path1, image2, path2, imageViewState);
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
