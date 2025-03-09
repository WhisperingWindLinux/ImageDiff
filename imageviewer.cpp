#include "imageviewer.h"

#include <QtGui/qevent.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qstatusbar.h>
#include <qfiledialog.h>
#include <qmainwindow.h>
#include <pixelsabsolutevaluecomparator.h>
#include <qmessagebox.h>
#include <imagecomparator.h>
#include <qapplication.h>
#include <qprocess.h>
#include <pixelsbrightnesscomparator.h>
#include <contrastcomporator.h>
#include <colorssaturationcomporator.h>


ImageViewer::ImageViewer(MainWindow *parent)
    : QGraphicsView(parent),
    currentImageIndex(0),
    scaleFactor(1.0),
    parent(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}

void ImageViewer::showImagesBeingCompared(QPixmap& image1,
                                          QString path1,
                                          QPixmap& image2,
                                          QString path2)
{
    firstImagePath = path1;
    secondImagePath = path2;
    firstImage = scene->addPixmap(image1);
    secondImage = scene->addPixmap(image2);
    parent->showStatusMessage(path1);
    secondImage->setVisible(false);
    setSceneRect(firstImage->boundingRect());
}

void ImageViewer::showComparisonImage(QPixmap &image, QString description) {
    if (comparisonImage != nullptr) {
        scene->removeItem(comparisonImage);
        comparisonImage = nullptr;
    }
    comparisonImage = scene->addPixmap(image);
    firstImage->setVisible(false);
    secondImage->setVisible(false);
    comparisonImage->setVisible(true);
    parent->showStatusMessage(description);
}

void ImageViewer::toggleImage() {

    if (firstImage == nullptr || secondImage == nullptr) {
        return;
    }

    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (comparisonImage != nullptr) {
        scene->removeItem(comparisonImage);
        comparisonImage = nullptr;
        currentImageIndex == 1 ?
            currentImageIndex = 0 :
            currentImageIndex = 1;
    }

    if (currentImageIndex == 0) {
        firstImage->setVisible(false);
        secondImage->setVisible(true);
        currentImageIndex = 1;
        parent->showStatusMessage(secondImagePath);
    } else {
        firstImage->setVisible(true);
        secondImage->setVisible(false);
        currentImageIndex = 0;
        parent->showStatusMessage(firstImagePath);
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
