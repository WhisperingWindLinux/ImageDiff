#include "imageviewer.h"

#include "imageviewstate.h"

#include <QtCore/qmimedata.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qstatusbar.h>
#include <qfiledialog.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qprocess.h>
#include <qlabel.h>


ImageViewer::ImageViewer(MainWindow *parent)
    : QGraphicsView(parent),
    currentImageIndex(0),
    scaleFactor(1.0),
    parent(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    setMouseTracking(true);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

ImageViewer::~ImageViewer() {
    if (firstImage != nullptr) {
        scene->removeItem(firstImage);
        delete firstImage;
        firstImage = nullptr;
    }
    if (secondImage != nullptr) {
        scene->removeItem(secondImage);
        delete secondImage;
        secondImage = nullptr;
    }
    if (comparisonImage != nullptr) {
        scene->removeItem(comparisonImage);
        delete comparisonImage;
        comparisonImage = nullptr;
    }
    if (lastMousEvent != nullptr) {
        delete lastMousEvent;
        lastMousEvent = nullptr;
    }
}

/* Color Picker is run { */

void ImageViewer::onColorPickerStatusChanged(bool isActivate) {
    if (isActivate) {
        isRgbTrackingActive = true;
        parent->onRgbValueUnderCursonChanged({firstImageName, -1, -1, -1}, {secondImageName, -1, -1, -1});
    } else {
        isRgbTrackingActive = false;
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Zoom { */

void ImageViewer::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}

void ImageViewer::zoomIn() {
    scale(1.25, 1.25);
    scaleFactor *= 1.25;
}

void ImageViewer::zoomOut() {
    scale(0.8, 0.8);
    scaleFactor *= 0.8;
}

void ImageViewer::actualSize() {
    resetTransform();
    scaleFactor = 1.0;
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Show images in QGraphicsView { */

void ImageViewer::showTwoImagesBeingCompared(QPixmap& image1,
                                          QString path1,
                                          QPixmap& image2,
                                          QString path2,
                                          std::shared_ptr<ImageViewState> imageViewState
                                          )
{
    firstImagePath = path1;
    secondImagePath = path2;
    firstImageName = QFileInfo(path1).baseName();
    secondImageName = QFileInfo(path2).baseName();
    firstImage = scene->addPixmap(image1);
    secondImage = scene->addPixmap(image2);
    parent->showStatusMessage(path1);
    secondImage->setVisible(false);
    if (imageViewState != nullptr) {
        centerOn(imageViewState.get()->rect.center());
        scale(imageViewState.get()->scaleFactor, imageViewState.get()->scaleFactor);
        scaleFactor = imageViewState.get()->scaleFactor;
        if (currentImageIndex != imageViewState.get()->imageIndex) {
            toggleImage();
        }
        onColorPickerStatusChanged(imageViewState->isRgbTrackingActive);
    } else {
        setSceneRect(firstImage->boundingRect());
    }
}

void ImageViewer::showImageFromComparator(QPixmap &image, QString description) {
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

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/*  Switching between images displayed in QGraphicsView.
 *  There are two images that the user compares,
 *  and the switching occurs between them.
 *  The result of the comparator's work is a third image,
 *  which can also be displayed in QGraphicsView
 *  but is not involved in the switching.
 *  It can only be displayed by running the corresponding
 *  comparator, which will compare the two images
 *  and return the comparison result as an image.
 *  {   */

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

    mouseMoveEvent(lastMousEvent);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/*  If a filter is applied to the two images being compared and displayed in QGraphicsView,
 *  they can change. For example, the Red Channel filter removes the blue and green channels.
 *  Filters are applied directly to the compared images. If the user selects
 *  "Show original images" from the menu, the effects of the filters will be discarded.
 *  To achieve this, the original images will be reloaded from disk. To preserve
 *  the scrolling parameters, zoom level, and current image index (0 or 1), we save them
 *  in a structure so they can be reapplied after loading the original images from the disk.
 *  {  */

ImageViewState ImageViewer::getCurrentState() {
    QRectF rect = mapToScene(viewport()->geometry()).boundingRect();
    return ImageViewState(rect, scaleFactor, currentImageIndex, isRgbTrackingActive);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Saving an image (or an area of the image) to disk { */


// Implementation of a method to capture the image displayed
// in QGraphicsView, taking into account the current scale (zoom)
// and visible area. In other words, everything outside the
// currently visible area will be cropped.
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

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Events { */

void ImageViewer::mouseMoveEvent(QMouseEvent* event) {

    if (event == nullptr) {
        return;
    }

    QGraphicsView::mouseMoveEvent(event);

    lastMousEvent = event->clone();

    if (selecting) {
        // Zoom to selection.
        // Update the selection rectangle as the user drags the mouse
        QPoint currentPoint = event->pos();
        selectionRect = QRect(selectionStart, currentPoint).normalized(); // Create a normalized rectangle
        viewport()->update(); // Request a repaint of the view
    }

    if (!isRgbTrackingActive) {
        return;
    }

    if (comparisonImage != nullptr) {
        return;
    }

    // Implement RGB values tracking under the mouse cursor.
    // It needs for Color Picker.

    QString imageName = "error: unknown image";
    if (comparisonImage != nullptr) {
        imageName = "comparison result image";
    } else if (currentImageIndex == 0) {
        imageName = firstImageName;
    } else if (currentImageIndex == 1) {
        imageName = secondImageName;
    }

    // Convert mouse position to scene coordinates
    QPointF scenePos = mapToScene(event->pos());

    // Get the item under the cursor (if any)
    QGraphicsItem* item = scene->itemAt(scenePos, transform());
    if (item) {
        // Check if the item is a pixmap (image)
        QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        if (pixmapItem) {
            // Get the pixmap (image) from the item
            const QPixmap pixmap = pixmapItem->pixmap();
            const QImage visibleImage = pixmap.toImage();

            // Convert scene coordinates to image coordinates
            QPointF itemPos = pixmapItem->mapFromScene(scenePos);
            int x = static_cast<int>(itemPos.x());
            int y = static_cast<int>(itemPos.y());

            // Ensure the coordinates are within the image bounds
            if (x >= 0 && x < visibleImage.width() && y >= 0 && y < visibleImage.height()) {
                // Get the color of the pixel
                QColor colorOfVisibleImage = visibleImage.pixelColor(x, y);
                QColor colorOfHiddenImage;
                QString visibleImageName, hiddenImageName;
                if (currentImageIndex == 0) {
                    colorOfHiddenImage = secondImage->pixmap().toImage().pixelColor(x, y);
                    visibleImageName = firstImageName;
                    hiddenImageName = secondImageName;
                } else {
                    colorOfHiddenImage = firstImage->pixmap().toImage().pixelColor(x, y);
                    visibleImageName = secondImageName;
                    hiddenImageName = firstImageName;
                }

                RgbValue rgbValueOfVisibleImage = { visibleImageName,
                                                    colorOfVisibleImage.red(),
                                                    colorOfVisibleImage.green(),
                                                    colorOfVisibleImage.blue()
                };
                RgbValue rgbValueOfHiddenImage = { hiddenImageName,
                                                   colorOfHiddenImage.red(),
                                                   colorOfHiddenImage.green(),
                                                   colorOfHiddenImage.blue()
                };
                parent->onRgbValueUnderCursonChanged(rgbValueOfVisibleImage, rgbValueOfHiddenImage);
            }
        }
    }
}

// Implement zoom to selection
void ImageViewer::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && event->modifiers() & Qt::ShiftModifier) {
        selecting = true;
        selectionStart = event->pos(); // Save the starting point of the selection in view coordinates
        selectionRect = QRect();       // Reset the selection rectangle
        setDragMode(NoDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

// Implement zoom to selection
void ImageViewer::mouseReleaseEvent(QMouseEvent *event) {
    if (selecting && event->button() == Qt::LeftButton) {
        selecting = false;

        // Convert the selection rectangle from view coordinates to scene coordinates
        QRectF sceneSelectionRect = mapToScene(selectionRect).boundingRect();

        // Zoom into the selected area
        if (!sceneSelectionRect.isEmpty() && sceneSelectionRect.isValid() && event->modifiers() & Qt::ShiftModifier) {
            fitInView(sceneSelectionRect, Qt::KeepAspectRatio); // Adjust view to fit the selected area
        }

        selectionRect = QRect(); // Clear the selection rectangle
        viewport()->update();    // Request a repaint of the view
        setDragMode(ScrollHandDrag);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

// Implement zoom to selection
void ImageViewer::paintEvent(QPaintEvent *event) {
    // Call base class paint event first
    QGraphicsView::paintEvent(event);

    // Draw the selection rectangle if it exists
    if (!selectionRect.isNull()) {
        QPainter painter(viewport());
        painter.setPen(QPen(Qt::blue, 1, Qt::DashLine)); // Dashed blue border for selection rectangle
        painter.setBrush(QBrush(Qt::transparent));      // Transparent fill
        painter.drawRect(selectionRect);
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
