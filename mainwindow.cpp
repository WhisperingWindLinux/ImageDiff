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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , viewer(nullptr)
{
    ui->setupUi(this);

    connect(ui->actionOpenImages, &QAction::triggered, this, &MainWindow::actionOpenImages_triggered);
    connect(ui->actionCloseImages, &QAction::triggered, this, &MainWindow::actionCloseImages_triggered);
    connect(ui->actionSwitchBetweenImages, &QAction::triggered, this, &MainWindow::actionSwitchBetweenImages_triggered);
    connect(ui->actionShowAbsolutePixelsValueDifference, &QAction::triggered, this, &MainWindow::actionShowAbsolutePixelsValueDifference_triggered);
    connect(ui->actionShowPixelsBrigthnessDifference, &QAction::triggered, this, &MainWindow::actionShowPixelsBrigthnessDifference_triggered);
    connect(ui->actionShowPixelsSaturationDifference, &QAction::triggered, this, &MainWindow::actionShowPixelsSaturationDifference_triggered);
    connect(ui->actionShowDifferenceAsImage, &QAction::triggered, this, &MainWindow::actionShowDifferenceAsImage_triggered);
    connect(ui->actionShowPixeslContrastDifference, &QAction::triggered, this, &MainWindow::actionShowPixeslContrastDifference_triggered);

    setWindowTitle("Image Diff");
    resize(1380, 820);

     ui->menuComparision->setDisabled(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showStatusMessage(QString &message)
{
    statusBar()->showMessage(message);
}

void MainWindow::actionOpenImages_triggered() {
    actionCloseImages_triggered();

    viewer = new ImageViewer(this);
    bool isLoaded = viewer->loadImages();

    if (isLoaded) {
        setCentralWidget(viewer);
        ui->menuComparision->setDisabled(false);
    }
    else {
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
}

void MainWindow::actionShowAbsolutePixelsValueDifference_triggered() {
    if (viewer == nullptr) {
        return;
    }
    viewer->showAbsolutePixelsValueDifference();
}

void MainWindow::actionShowDifferenceAsImage_triggered() {
    if (viewer == nullptr) {
        return;
    }
    viewer->showDifferenceAsImage();
}

void MainWindow::actionShowPixelsBrigthnessDifference_triggered() {
    if (viewer == nullptr) {
        return;
    }
    viewer->showPixelsBrigthnessDifference();
}

void MainWindow::actionShowPixelsSaturationDifference_triggered() {
    if (viewer == nullptr) {
        return;
    }
    viewer->showPixelsSaturationDifference();
}

void MainWindow::actionShowPixeslContrastDifference_triggered() {
    if (viewer == nullptr) {
        return;
    }
    viewer->showPixeslContrastDifference();
}

void MainWindow::actionSwitchBetweenImages_triggered() {
    if (viewer == nullptr) {
        return;
    }
    viewer->toggleImage();
}
