#include "externalimageviewerdialog.h"

#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <qsettings.h>
#include <QtWidgets/qscrollbar.h>
#include <presentation/views/externalgraphicsview.h>

ExternalImageViewerDialog::ExternalImageViewerDialog(const QPixmap &image, const QString &description, QWidget *parent)
    : QDialog(parent) {
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setModal(true);
    setWindowTitle(description);

    graphicsView = new ExternalGraphicsView(image);

    menuBar = new QMenuBar(this);
    setupMenu();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMenuBar(menuBar);
    layout->addWidget(graphicsView);
    setLayout(layout);
}

void ExternalImageViewerDialog::showEvent(QShowEvent*) {
    restoreWindowState();
    graphicsView->setToFitImageInView();
}

void ExternalImageViewerDialog::setupMenu() {
    // File menu
    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *closeAction = fileMenu->addAction("Close");
    connect(closeAction, &QAction::triggered, this, &ExternalImageViewerDialog::close);

    // View menu
    QMenu *viewMenu = menuBar->addMenu("View");

    QAction *fitInViewAction = viewMenu->addAction("Fit In View");
    fitInViewAction->setShortcut(Qt::Key_F);
    connect(fitInViewAction, &QAction::triggered, graphicsView, &ExternalGraphicsView::setToFitImageInView);

    QAction *zoomInAction = viewMenu->addAction("Zoom In");
#ifdef Q_OS_MAC
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
#else
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
#endif
    connect(zoomInAction, &QAction::triggered, graphicsView, &ExternalGraphicsView::zoomIn);

    QAction *zoomOutAction = viewMenu->addAction("Zoom Out");
#ifdef Q_OS_MAC
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
#else
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
#endif
    connect(zoomOutAction, &QAction::triggered, graphicsView, &ExternalGraphicsView::zoomOut);
}

void ExternalImageViewerDialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
        return;
    }
    QDialog::keyPressEvent(event);
}

void ExternalImageViewerDialog::saveWindowState() {
    QSettings settings("com.WhisperingWind", "TwinPix");
    settings.beginGroup("ExternalImageViewerDialog");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
}

void ExternalImageViewerDialog::restoreWindowState() {
    QSettings settings("com.WhisperingWind", "TwinPix");
    settings.beginGroup("ExternalImageViewerDialog");
    restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();
}

void ExternalImageViewerDialog::closeEvent(QCloseEvent *event) {
    saveWindowState();
    event->accept();
}
