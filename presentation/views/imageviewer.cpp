#include "imageviewer.h"

#include <QtCore/qmimedata.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qstatusbar.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qprocess.h>
#include <qlabel.h>
#include <QGraphicsView>
#include <presentation/mainwindow.h>
#include <business/utils/imagesinfo.h>

ImageViewer::ImageViewer(MainWindow *parent)
    : QGraphicsView(parent),
    parent(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    QColor backgroundColor = QApplication::palette().color(QPalette::Window);
    setBackgroundBrush(backgroundColor);

    setMouseTracking(true);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    cleanUp();
}

ImageViewer::~ImageViewer() {
    cleanUp();
}

/* Color Picker is run { */

void ImageViewer::onColorUnderCursorTrackingStatusChanged(bool isActivate) {
    if (isActivate) {
        isColorUnderCursorTrackingActive = true;
        getPixelColorUnderCursor(lastCursorPos);
    } else {
        isColorUnderCursorTrackingActive = false;
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
    event->accept();
}

void ImageViewer::zoomIn() {
    setCenterToViewRectCenter();
    scale(1.25, 1.25);
    scaleFactor *= 1.25;
}

void ImageViewer::zoomOut() {
    setCenterToViewRectCenter();
    scale(0.8, 0.8);
    scaleFactor *= 0.8;
}

void ImageViewer::setCenterToViewRectCenter() {
    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();
    centerOn(viewRect.center());
}

void ImageViewer::setToActualSize() {
    resetTransform();
    scaleFactor = 1.0;
}

void ImageViewer::setToFitImageInView() {
    fitInView(firstDisplayedImage, Qt::KeepAspectRatio);
    fitInView(secondDisplayedImage, Qt::KeepAspectRatio);
    if (comparatorResultDisplayedImage != nullptr) {
        fitInView(comparatorResultDisplayedImage, Qt::KeepAspectRatio);
    }
    scaleFactor = 1.0;
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Show images in QGraphicsView { */

void ImageViewer::displayImages(const ImagesPtr images)
{
    if (images == nullptr) {
        return;
    }

    ImagesInfo info { images };

    QString image1Name = info.getFirstImageBaseName();
    QString image2Name = info.getSecondImageBaseName();

    firstImagePath = images->path1;
    secondImagePath = images->path2;
    firstImageName = image1Name;
    secondImageName = image2Name;
    
    firstDisplayedImage = scene->addPixmap(images->image1);
    secondDisplayedImage = scene->addPixmap(images->image2);
    parent->showStatusMessage(firstImagePath);
    secondDisplayedImage->setVisible(false);
    setToFitImageInView();
}

void ImageViewer::showImageFromComparator(const QPixmap &image, const QString &description) {
    if (image.isNull()) {
        return;
    }
    if (comparatorResultDisplayedImage != nullptr) {
        scene->removeItem(comparatorResultDisplayedImage);
        comparatorResultDisplayedImage = nullptr;
    }
    comparatorResultDisplayedImage = scene->addPixmap(image);
    firstDisplayedImage->setVisible(false);
    secondDisplayedImage->setVisible(false);
    comparatorResultDisplayedImage->setVisible(true);
    parent->showStatusMessage(description);
}

void ImageViewer::replaceDisplayedImages(const QPixmap& image1, const QPixmap& image2) {

    if (image1.isNull() || image2.isNull()) {
        return;
    }

    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (firstDisplayedImage != nullptr) {
        scene->removeItem(firstDisplayedImage);
        firstDisplayedImage = nullptr;
    }
    if (secondDisplayedImage != nullptr) {
        scene->removeItem(secondDisplayedImage);
        secondDisplayedImage = nullptr;
    }
    if (comparatorResultDisplayedImage != nullptr) {
        scene->removeItem(comparatorResultDisplayedImage);
        comparatorResultDisplayedImage = nullptr;
    }
    firstDisplayedImage = scene->addPixmap(image1);
    secondDisplayedImage = scene->addPixmap(image2);
    if (currentImageIndex == 0) {
        secondDisplayedImage->setVisible(false);
        parent->showStatusMessage(firstImagePath);
    } else {
        firstDisplayedImage->setVisible(false);
        parent->showStatusMessage(secondImagePath);
    }
    centerOn(viewRect.center());
    if (lastCursorPos) {
        getPixelColorUnderCursor(lastCursorPos.value());
    }
}

bool ImageViewer::hasActiveSession() {
    return (firstDisplayedImage != nullptr && secondDisplayedImage != nullptr);
}

void ImageViewer::cleanUp() {
    if (firstDisplayedImage != nullptr) {
        scene->removeItem(firstDisplayedImage);
        firstDisplayedImage = nullptr;
    }
    if (secondDisplayedImage != nullptr) {
        scene->removeItem(secondDisplayedImage);
        secondDisplayedImage = nullptr;
    }
    if (comparatorResultDisplayedImage != nullptr) {
        scene->removeItem(comparatorResultDisplayedImage);
        comparatorResultDisplayedImage = nullptr;
    }
    firstImagePath = "";
    secondImagePath = "";
    firstImageName = "";
    secondImageName = "";
    currentImageIndex = 0;
    scaleFactor = 1.0;
    isColorUnderCursorTrackingActive = false;
    lastCursorPos = std::nullopt;
    selecting = false;
    isZoomToSelectionEnabled = false;
    selectionStart = {};
    selectionRect = {};
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
    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (comparatorResultDisplayedImage != nullptr) {
        scene->removeItem(comparatorResultDisplayedImage);
        comparatorResultDisplayedImage = nullptr;
        currentImageIndex == 1 ?
            currentImageIndex = 0 :
            currentImageIndex = 1;
    }

    if (currentImageIndex == 0) {
        firstDisplayedImage->setVisible(false);
        secondDisplayedImage->setVisible(true);
        currentImageIndex = 1;
        parent->showStatusMessage(secondImagePath);
    } else {
        firstDisplayedImage->setVisible(true);
        secondDisplayedImage->setVisible(false);
        currentImageIndex = 0;
        parent->showStatusMessage(firstImagePath);
    }

    centerOn(viewRect.center());
    if (lastCursorPos) {
        getPixelColorUnderCursor(lastCursorPos.value());
    }
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
    bool isComparisonImageShowing = (comparatorResultDisplayedImage != nullptr);
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
    bool isComparisonImageShowing = (comparatorResultDisplayedImage != nullptr);
    if (isComparisonImageShowing) {
        return SaveImageInfo(SaveImageInfoType::ComparisonImage, comparatorResultDisplayedImage->pixmap());
    } else if (currentImageIndex == 0) {
        return SaveImageInfo(SaveImageInfoType::FirstImage, firstDisplayedImage->pixmap());
    } else if (currentImageIndex == 1) {
        return SaveImageInfo(SaveImageInfoType::SecondImage, secondDisplayedImage->pixmap());
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

void ImageViewer::passCropedImageToOtherAppInstance(QRectF rect) {
    QRect selectionRect = rect.toAlignedRect();

    auto firstPixmap = firstDisplayedImage->pixmap();
    auto secondPixmap = secondDisplayedImage->pixmap();
    QRect boundedRect1 = selectionRect.intersected(firstPixmap.rect());
    QPixmap croppedPixmap1 = firstPixmap.copy(boundedRect1);
    QRect boundedRect2 = selectionRect.intersected(secondPixmap.rect());
    QPixmap croppedPixmap2 = secondPixmap.copy(boundedRect2);

    QString path1 = savePixmapToTempDir(croppedPixmap1, firstImageName);
    QString path2 = savePixmapToTempDir(croppedPixmap2, secondImageName);

    parent->openImagesInOtherAppInstance(path1, path2);
}

QString ImageViewer::savePixmapToTempDir(const QPixmap &pixmap, const QString &fileName) {
    QString tempDir = QDir::tempPath();

    QString filePath = QDir(tempDir).filePath(QString(fileName) + ".png");

    if (pixmap.save(filePath)) {
        return filePath;
    } else {
        return {};
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Events { */

void ImageViewer::mouseMoveEvent(QMouseEvent* event) {
    if (event == nullptr) {
        return;
    }
    lastCursorPos = event->pos();
    QGraphicsView::mouseMoveEvent(event);

    if (selecting) {
        // Zoom to selection.
        // Update the selection rectangle as the user drags the mouse
        QPoint currentPoint = event->pos();
        selectionRect = QRect(selectionStart, currentPoint).normalized();
        viewport()->update();
    }

    // Implement RGB values tracking under the mouse cursor. It needs for Color Picker.
    getPixelColorUnderCursor(event->pos());
}

void ImageViewer::getPixelColorUnderCursor(std::optional<QPoint> cursorPos) {
    if (!isColorUnderCursorTrackingActive || !cursorPos) {
        return;
    }

    // Convert mouse position to scene coordinates
    QPointF scenePos = mapToScene(cursorPos.value());

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
                QColor colorOfVisibleImage, colorOfHiddenImage;
                QString visibleImageName, hiddenImageName;
                if (currentImageIndex == 0) {
                    colorOfVisibleImage = firstDisplayedImage->pixmap().toImage().pixelColor(x, y);
                    colorOfHiddenImage = secondDisplayedImage->pixmap().toImage().pixelColor(x, y);
                    visibleImageName = firstImageName;
                    hiddenImageName = secondImageName;
                } else {
                    colorOfVisibleImage = secondDisplayedImage->pixmap().toImage().pixelColor(x, y);
                    colorOfHiddenImage = firstDisplayedImage->pixmap().toImage().pixelColor(x, y);
                    visibleImageName = secondImageName;
                    hiddenImageName = firstImageName;
                }
                fillPixelColorValues(visibleImageName, colorOfVisibleImage, hiddenImageName, colorOfHiddenImage);
            }
        }
    }
}

void ImageViewer::fillPixelColorValues(QString visibleImageName,
                                       QColor colorOfVisibleImage,
                                       QString hiddenImageName,
                                       QColor colorOfHiddenImage
                                       )
{
    ImagePixelColor pixelValueOfVisibleImage = {
        visibleImageName,
        colorOfVisibleImage.red(),
        colorOfVisibleImage.green(),
        colorOfVisibleImage.blue()
    };

    ImagePixelColor pixelValueOfHiddenImage = {
            hiddenImageName,
            colorOfHiddenImage.red(),
            colorOfHiddenImage.green(),
            colorOfHiddenImage.blue()
    };
    parent->onColorUnderCursorChanged(pixelValueOfVisibleImage, pixelValueOfHiddenImage);
}

// Implement zoom to selection
void ImageViewer::mousePressEvent(QMouseEvent *event) {
    auto shiftModifier = event->modifiers() & Qt::ShiftModifier;
    auto controlModifier = event->modifiers() & Qt::ControlModifier;
    if (event->button() == Qt::LeftButton && (shiftModifier || controlModifier)) {
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
        if (!sceneSelectionRect.isEmpty() &&
            sceneSelectionRect.isValid() &&
            event->modifiers() & Qt::ShiftModifier
            )
        {
            fitInView(sceneSelectionRect, Qt::KeepAspectRatio); // Adjust view to fit the selected area
        }
        else if (!sceneSelectionRect.isEmpty() &&
                 sceneSelectionRect.isValid() &&
                 event->modifiers() & Qt::ControlModifier
                 )
        {
            passCropedImageToOtherAppInstance(sceneSelectionRect);
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
