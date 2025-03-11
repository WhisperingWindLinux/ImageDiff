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

#include <gui/imagegeometry.h>
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
    connect(ui->actionColorPicker, &QAction::toggled, this, &MainWindow::actionColorPicker_triggered);
    connect(ui->actionShowOriginalImage, &QAction::triggered, this, &MainWindow::actionShowOriginalImage_triggered);

    colorPanel = new ColorInfoPanel();
    colorPanel->hide();

    setWindowTitle("Image Diff");
    resize(1380, 820);
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

        if (type == ImageProcessorType::Comporator) {
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

void MainWindow::closeEvent(QCloseEvent *event) {
    if (colorPanel != nullptr) {
        colorPanel->close();
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
    if (colorPanel->isVisible()) {
        ui->actionColorPicker->trigger();
    }
    if (viewer != nullptr) {
        setCentralWidget(nullptr);
        delete viewer;
        viewer = nullptr;
    }
    ui->menuComparators->setDisabled(true);
    ui->menuTransftomers->setDisabled(true);
    ui->menuImageAnalysis->setDisabled(true);
    showStatusMessage("");
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

void MainWindow::actionColorPicker_triggered(bool isTogled) {
    if (viewer == nullptr) {
        return;
    }
    if (isTogled) {
        viewer->onColorPickerStatusChanged(true);
        colorPanel->show();
    } else {
        viewer->onColorPickerStatusChanged(false);
        colorPanel->hide();
    }
}

void MainWindow::actionShowOriginalImage_triggered() {
    comparisionInteractor->realoadImagesFromDisk();
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
    } catch (std::exception &e) {
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

void MainWindow::onRgbValueUnderCursonChanged(QString imageName, int r, int g, int b) {
    colorPanel->updateColor(imageName, r, g, b);
}

// AMainWindowCallbacks interface

void MainWindow::onImagesBeingComparedLoaded(QPixmap& image1,
                                             QString path1,
                                             QPixmap& image2,
                                             QString path2,
                                             bool usePreviousImageGeometry
                                             )
{
    std::shared_ptr<ImageGeometry> imageGeometry = nullptr;
    if (usePreviousImageGeometry && viewer) {
        imageGeometry = std::make_shared<ImageGeometry>(viewer->getImageGometry());
    }
    if (viewer != nullptr) {
        setCentralWidget(nullptr);
        delete viewer;
        viewer = nullptr;
    }
    viewer = new ImageViewer(this);
    setCentralWidget(viewer);
    viewer->showImagesBeingCompared(image1, path1, image2, path2, imageGeometry);
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











