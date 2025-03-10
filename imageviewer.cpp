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

/*
   Implementation of a method to capture the image displayed
   in QGraphicsView, taking into account the current scale (zoom)
   and visible area. In other words, everything outside the
   currently visible area will be cropped.
  */
SaveImageInfo ImageViewer::getCurrentVisiableArea() {
    if (scene == nullptr) {
        return {};
    }

    QPixmap pixmap = ImageViewer::getVisiblePixmap(this);


    SaveImageInfoType contentType = SaveImageInfoType::None;
    bool isComparisonImageShowing = (comparisonImage != nullptr);
    if (isComparisonImageShowing) {
        contentType = SaveImageInfoType::ComparisonImageArea;
    } else if (currentImageIndex == 0) {
        contentType = SaveImageInfoType::FirstImageArea;
    } else if (currentImageIndex == 1) {
        contentType = SaveImageInfoType::SecondImageArea;
    }

    return SaveImageInfo(contentType, pixmap);
}

SaveImageInfo ImageViewer::getImageShowedOnTheScreen() {

    bool isComparisonImageShowing = (comparisonImage != nullptr);
    if (isComparisonImageShowing) {
        return SaveImageInfo(SaveImageInfoType::ComparisonImage, comparisonImage->pixmap());
    } else if (currentImageIndex == 0) {
        return SaveImageInfo(SaveImageInfoType::FirstImage, firstImage->pixmap());
    } else if (currentImageIndex == 1) {
        return SaveImageInfo(SaveImageInfoType::SecondImage, secondImage->pixmap());
    }
    return {};
}

// Function to get the visible portion of the image in QGraphicsView
QPixmap ImageViewer::getVisiblePixmap(QGraphicsView* view) {
    if (!view || !view->scene()) {
        return QPixmap(); // Return an empty QPixmap if the view or scene is not set
    }

    // Get the visible area in scene coordinates
    QRectF visibleSceneRect = view->mapToScene(view->viewport()->geometry()).boundingRect();

    // Assume the image is added as a QGraphicsPixmapItem
    auto items = view->scene()->items(visibleSceneRect);
    QGraphicsPixmapItem* pixmapItem = nullptr;

    for (auto it = items.begin(); it != items.end(); ++it) {
        pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>((*it));
        if (pixmapItem) {
            break; // Take the first image found
        }
    }

    if (!pixmapItem) {
        return QPixmap(); // If no image is found, return an empty QPixmap
    }

    // Get the original image (pixmap)
    QPixmap originalPixmap = pixmapItem->pixmap();

    // Convert the visible area from scene coordinates to image coordinates
    QRectF pixmapRect = pixmapItem->sceneBoundingRect(); // Image boundaries in scene coordinates
    QRectF intersection = visibleSceneRect.intersected(pixmapRect); // Intersection of visible area and image

    if (intersection.isEmpty()) {
        return QPixmap(); // If there is no intersection, return an empty QPixmap
    }

    // Transform coordinates from scene to image pixel coordinates
    QRectF sourceRect(
        (intersection.left() - pixmapRect.left()) / pixmapRect.width() * originalPixmap.width(),
        (intersection.top() - pixmapRect.top()) / pixmapRect.height() * originalPixmap.height(),
        intersection.width() / pixmapRect.width() * originalPixmap.width(),
        intersection.height() / pixmapRect.height() * originalPixmap.height()
        );

    // Create a new pixmap with the size of the visible area
    QSize targetSize = view->viewport()->size();
    QPixmap result(targetSize);
    result.fill(Qt::transparent); // Fill with a transparent color

    // Draw the portion of the image on the resulting QPixmap while preserving proportions
    QPainter painter(&result);
    painter.drawPixmap(
        QRectF(0, 0, targetSize.width(), targetSize.height()), // Where to draw (entire result)
        originalPixmap,
        sourceRect // Which part of the original image to draw
        );
    painter.end();

    return result;
}










