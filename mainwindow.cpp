#include "aboutdialog.h"
#include "colorinfopanel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

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
#include <ImageViewer.h>
#include <comparisionmanager.h>
#include <qmessagebox.h>
#include <savefileinfo.h>
#include <QDockWidget>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    comparisionInteractor = new ComparisionInteractor(this);
    buildMenus();

    ui->menuComparision->setDisabled(true);
    ui->menuImage_Analysis->setDisabled(true);

    connect(ui->actionOpenImages, &QAction::triggered, this, &MainWindow::actionOpenImages_triggered);
    connect(ui->actionCloseImages, &QAction::triggered, this, &MainWindow::actionCloseImages_triggered);
    connect(ui->actionSwitchBetweenImages, &QAction::triggered, this, &MainWindow::actionSwitchBetweenImages_triggered);
    connect(ui->actionSaveImageAs, &QAction::triggered, this, &MainWindow::actionSaveImageAs_triggered);
    connect(ui->actionSaveVisibleAreaAs, &QAction::triggered, this, &MainWindow::actionSaveVisibleAreaAs_triggered);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::actionAbout_triggered);
    connect(ui->actionColor_Picker, &QAction::toggled, this, &MainWindow::actionColorPicker_triggered);

    colorPanel = new ColorInfoPanel();
    colorPanel->hide();

    setWindowTitle("Image Diff");
    resize(1380, 820);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::buildMenus() {
    ComparisionManager *manager = ComparisionManager::instance();
    auto comporatorsInfo = manager->allComporatorsInfo();

    // an image comporators
    for (auto it = comporatorsInfo.begin(); it != comporatorsInfo.end(); ++it) {
        if ((*it).contentType == ComporatorContentType::Image) {
            QString name = (*it).name;
            QString hotkey = (*it).hotkey;
            QMenu *menu = ui->menuComparision;

            QAction *newAction = menu->addAction(name);
            newAction->setData(name);
            newAction->setShortcut(QKeySequence(hotkey));

            connect(newAction, &QAction::triggered, this, &MainWindow::actionImageComparatorsMenuItem_triggered);
        }
    }

    // TBD a video comporators
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
        ui->menuComparision->setDisabled(false);
        ui->menuImage_Analysis->setDisabled(false);
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
    ui->menuComparision->setDisabled(true);
    ui->menuImage_Analysis->setDisabled(true);
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
    comparisionInteractor->onImageComporatorShouldBeCalled(action->data());
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
    } else {
        // QMessageBox::information(this, "Success", "Image saved successfully.");
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
                                             QString path2)
{
    viewer = new ImageViewer(this);
    setCentralWidget(viewer);
    viewer->showImagesBeingCompared(image1, path1, image2, path2);
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
