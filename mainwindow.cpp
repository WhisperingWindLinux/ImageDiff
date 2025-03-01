#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QSplitter>
#include <QVBoxLayout>
#include <qgraphicsview.h>
#include <QApplication>
#include <QMainWindow>
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
{
    ui->setupUi(this);

    viewer = new ImageViewer(this);
    setCentralWidget(viewer);

    setWindowTitle("Image Diff");
    resize(800, 600);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showStatusMessage(QString &message)
{
    statusBar()->showMessage(message);
}
