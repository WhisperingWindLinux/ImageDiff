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

    mGraphicsView = new ExternalGraphicsView(image);

    mMenuBar = new QMenuBar(this);
    setupMenu();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMenuBar(mMenuBar);
    layout->addWidget(mGraphicsView);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
}

void ExternalImageViewerDialog::showEvent(QShowEvent*) {
    restoreWindowState();
    mGraphicsView->setToFitImageInView();
}

void ExternalImageViewerDialog::setupMenu() {
    // File menu
    QMenu *fileMenu = mMenuBar->addMenu("File");
    QAction *closeAction = fileMenu->addAction("Close");
    connect(closeAction, &QAction::triggered, this, &ExternalImageViewerDialog::close);

    // View menu
    QMenu *viewMenu = mMenuBar->addMenu("View");

    QAction *fitInViewAction = viewMenu->addAction("Fit In View");
    fitInViewAction->setShortcut(Qt::Key_F);
    connect(fitInViewAction, &QAction::triggered, mGraphicsView, &ExternalGraphicsView::setToFitImageInView);

    QAction *zoomInAction = viewMenu->addAction("Zoom In");
#ifdef Q_OS_MAC
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
#else
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
#endif
    connect(zoomInAction, &QAction::triggered, mGraphicsView, &ExternalGraphicsView::zoomIn);

    QAction *zoomOutAction = viewMenu->addAction("Zoom Out");
#ifdef Q_OS_MAC
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
#else
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
#endif
    connect(zoomOutAction, &QAction::triggered, mGraphicsView, &ExternalGraphicsView::zoomOut);
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
