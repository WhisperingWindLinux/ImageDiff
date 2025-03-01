#include "imageviewer.h"
#include <QtGui/qevent.h>
#include <QtWidgets/qgraphicsitem.h>
#include <MainWindow.h>
#include <QtWidgets/qstatusbar.h>
#include <qfiledialog.h>
#include <qmainwindow.h>

ImageViewer::ImageViewer(MainWindow *parent)
    : QGraphicsView(parent), currentImageIndex(0), scaleFactor(1.0), parent(parent) {
    scene = new QGraphicsScene(this);
    setScene(scene);

    loadImages();

    if (!images[0].isNull()) {
        pixmapItem1 = scene->addPixmap(images[0]);
        pixmapItem2 = scene->addPixmap(images[1]);
        parent->showStatusMessage(file1);
        pixmapItem2->setVisible(false);
        setSceneRect(pixmapItem1->boundingRect());
    }

    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ImageViewer::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_S) {
        toggleImage();
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}

void ImageViewer::loadImages() {
    file1 = QFileDialog::getOpenFileName(nullptr, "Open First Image", "", "Images (*.png)");
    file2 = QFileDialog::getOpenFileName(nullptr, "Open Second Image", "", "Images (*.png)");

    if (!file1.isEmpty() && !file2.isEmpty()) {
        images[0].load(file1);
        images[1].load(file2);

        if (images[0].size() != images[1].size()) {
            qDebug() << "Error: Images must have the same resolution!";
            exit(1);
        }
    } else {
        qDebug() << "Error: Both images must be selected!";
        exit(1);
    }
}

void ImageViewer::toggleImage() {
    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (currentImageIndex == 0) {
        pixmapItem1->setVisible(false);
        pixmapItem2->setVisible(true);
        currentImageIndex = 1;
        parent->showStatusMessage(file2);
    } else {
        pixmapItem1->setVisible(true);
        pixmapItem2->setVisible(false);
        currentImageIndex = 0;
        parent->showStatusMessage(file1);
    }

    centerOn(viewRect.center());
}

void ImageViewer::zoomIn() {
    scale(1.25, 1.25);
    scaleFactor *= 1.25;
}

void ImageViewer::zoomOut() {
    scale(0.8, 0.8);
    scaleFactor *= 0.8;
}
