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
#include <QTimer>
#include <QGraphicsView>
#include <presentation/mainwindow.h>
#include <business/utils/imagesinfo.h>


ImageViewer::ImageViewer(IDropListener *dropListener, MainWindow *parent)
    : QGraphicsView(parent),
    parent(parent),
    dropListener(dropListener)
{    
    customScene = nullptr;
    firstDisplayedImage = nullptr;
    secondDisplayedImage = nullptr;
    comparatorResultDisplayedImage = nullptr;

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
        sendPixelColorUnderCursor(lastCursorPos);
    } else {
        isColorUnderCursorTrackingActive = false;
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Zoom { */

void ImageViewer::wheelEvent(QWheelEvent *event) {
    if (!hasActiveSession()) {
        event->ignore();
        return;
    }
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
    sendPixelColorUnderCursor(lastCursorPos);
}

void ImageViewer::zoomOut() {
    setCenterToViewRectCenter();
    scale(0.8, 0.8);
    scaleFactor *= 0.8;
    sendPixelColorUnderCursor(lastCursorPos);
}

void ImageViewer::setCenterToViewRectCenter() {
    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();
    centerOn(viewRect.center());
}

void ImageViewer::setToActualSize() {
    if (!hasActiveSession()) {
        return;
    }
    resetTransform();
    scaleFactor = 1.0;
}

void ImageViewer::setToFitImageInView() {
    if (!hasActiveSession()) {
        return;
    }
    fitInView(firstDisplayedImage, Qt::KeepAspectRatio);
    fitInView(secondDisplayedImage, Qt::KeepAspectRatio);
    if (comparatorResultDisplayedImage != nullptr) {
        fitInView(comparatorResultDisplayedImage, Qt::KeepAspectRatio);
    }
    scaleFactor = 1.0;
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Show images in QGraphicsView { */

void ImageViewer::displayImages(const ImagesPtr images) {

    cleanUp();

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

    firstDisplayedImage = new GraphicsPixmapItem(images->image1, dropListener);
    secondDisplayedImage = new GraphicsPixmapItem(images->image2, dropListener);

    setAcceptDrops(true);
    customScene = new QGraphicsScene(this);
    setScene(customScene);

    customScene->addItem(firstDisplayedImage);
    customScene->addItem(secondDisplayedImage);

    parent->showStatusMessage(firstImagePath);
    secondDisplayedImage->setVisible(false);

    QTimer::singleShot(0, this, [this]() {
        setToFitImageInView();
    });
}

void ImageViewer::showImageFromComparator(const QPixmap &image, const QString &description) {
    if (image.isNull() || !hasActiveSession()) {
        return;
    }
    if (comparatorResultDisplayedImage != nullptr) {
        customScene->removeItem(comparatorResultDisplayedImage);
        comparatorResultDisplayedImage = nullptr;
    }

    comparatorResultDisplayedImage = new GraphicsPixmapItem(image, dropListener);
    customScene->addItem(comparatorResultDisplayedImage);
    firstDisplayedImage->setVisible(false);
    secondDisplayedImage->setVisible(false);
    comparatorResultDisplayedImage->setVisible(true);
    parent->showStatusMessage(description);
}

void ImageViewer::replaceDisplayedImages(const QPixmap& image1, const QPixmap& image2) {

    if (image1.isNull() || image2.isNull() || !hasActiveSession()) {
        return;
    }

    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (firstDisplayedImage != nullptr) {
        customScene->removeItem(firstDisplayedImage);
        firstDisplayedImage = nullptr;
    }
    if (secondDisplayedImage != nullptr) {
        customScene->removeItem(secondDisplayedImage);
        secondDisplayedImage = nullptr;
    }
    if (comparatorResultDisplayedImage != nullptr) {
        customScene->removeItem(comparatorResultDisplayedImage);
        comparatorResultDisplayedImage = nullptr;
    }

    firstDisplayedImage = new GraphicsPixmapItem(image1, dropListener);
    secondDisplayedImage = new GraphicsPixmapItem(image2, dropListener);
    customScene->addItem(firstDisplayedImage);
    customScene->addItem(secondDisplayedImage);

    if (currentImageIndex == 0) {
        secondDisplayedImage->setVisible(false);
        parent->showStatusMessage(firstImagePath);
    } else {
        firstDisplayedImage->setVisible(false);
        parent->showStatusMessage(secondImagePath);
    }
    centerOn(viewRect.center());
    if (lastCursorPos) {
        sendPixelColorUnderCursor(lastCursorPos.value());
    }
}

bool ImageViewer::hasActiveSession() {
    return (firstDisplayedImage != nullptr && secondDisplayedImage != nullptr);
}

void ImageViewer::cleanUp() {
    if (firstDisplayedImage != nullptr) {
        customScene->removeItem(firstDisplayedImage);
        firstDisplayedImage = nullptr;
    }
    if (secondDisplayedImage != nullptr) {
        customScene->removeItem(secondDisplayedImage);
        secondDisplayedImage = nullptr;
    }
    if (comparatorResultDisplayedImage != nullptr) {
        customScene->removeItem(comparatorResultDisplayedImage);
        comparatorResultDisplayedImage = nullptr;
    }
    if (customScene != nullptr) {
        setScene(nullptr);
        delete customScene;
        customScene = nullptr;
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
    if (!hasActiveSession()) {
        return;
    }
    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();

    if (comparatorResultDisplayedImage != nullptr) {
        customScene->removeItem(comparatorResultDisplayedImage);
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
        sendPixelColorUnderCursor(lastCursorPos.value());
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Saving an image (or an area of the image) to disk { */


// Implementation of a method to capture the image displayed
// in QGraphicsView, taking into account the current scale (zoom)
// and visible area. In other words, everything outside the
// currently visible area will be cropped.
SaveImageInfo ImageViewer::getCurrentVisiableArea() {
    if (!hasActiveSession()) {
        return {};
    }

    QPixmap pixmap = getVisiblePixmap();

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
    if (!hasActiveSession()) {
        return {};
    }
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
QPixmap ImageViewer::getVisiblePixmap() {

    // Get the visible area in scene coordinates
    QRectF visibleSceneRect = mapToScene(viewport()->geometry()).boundingRect();

    auto items = customScene->items(visibleSceneRect);
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
    QSize targetSize = viewport()->size();
    QPixmap result(targetSize);
    result.fill(Qt::transparent);

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

ImagesPtr ImageViewer::getCroppedImages(QRectF rect) {

    QRect selectionRect = rect.toAlignedRect();

    auto firstPixmap = firstDisplayedImage->pixmap();
    auto secondPixmap = secondDisplayedImage->pixmap();
    QRect boundedRect1 = selectionRect.intersected(firstPixmap.rect());
    QPixmap croppedPixmap1 = firstPixmap.copy(boundedRect1);
    QRect boundedRect2 = selectionRect.intersected(secondPixmap.rect());
    QPixmap croppedPixmap2 = secondPixmap.copy(boundedRect2);

    return std::make_shared<Images>(croppedPixmap1, croppedPixmap2,  firstImageName, secondImageName);
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Events { */

void ImageViewer::mouseMoveEvent(QMouseEvent* event) {
    if (event == nullptr || !hasActiveSession()) {
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
    sendPixelColorUnderCursor(event->pos());
}

void ImageViewer::sendPixelColorUnderCursor(std::optional<QPoint> cursorPos) {
    if (!isColorUnderCursorTrackingActive || !cursorPos || !hasActiveSession()) {
        return;
    }

    // Convert mouse position to scene coordinates
    QPointF scenePos = mapToScene(cursorPos.value());

    // Get the item under the cursor (if any)
    QGraphicsItem* item = customScene->itemAt(scenePos, transform());
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
                sendPixelColorValues(visibleImageName, colorOfVisibleImage, hiddenImageName, colorOfHiddenImage);
            }
        }
    }
}

void ImageViewer::sendPixelColorValues(const QString &visibleImageName,
                                       const QColor &colorOfVisibleImage,
                                       const QString &hiddenImageName,
                                       const QColor &colorOfHiddenImage
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
    if (!hasActiveSession()) {
        return;
    }
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
    if (!hasActiveSession()) {
        return;
    }
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
            ImagesPtr images = getCroppedImages(sceneSelectionRect);
            parent->onImagesCropped(images);
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

    if (!hasActiveSession()) {
        return;
    }
    // Draw the selection rectangle if it exists
    if (!selectionRect.isNull()) {
        QPainter painter(viewport());
        painter.setPen(QPen(Qt::blue, 1, Qt::DashLine)); // Dashed blue border for selection rectangle
        painter.setBrush(QBrush(Qt::transparent));       // Transparent fill
        painter.drawRect(selectionRect);
    }
}

void ImageViewer::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("text/uri-list") && dropListener) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void ImageViewer::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasUrls() || !dropListener) {
        event->ignore();
    } else {
        event->acceptProposedAction();
        QList<QUrl> urls = event->mimeData()->urls();
        dropListener->onDrop(urls);
    }
}

/* } =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
