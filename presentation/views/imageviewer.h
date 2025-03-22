#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "domain/valueobjects/images.h"
#include "graphicspixmapitem.h"
#include <domain/valueobjects/savefileinfo.h>

#include <qgraphicsview.h>

class MainWindow;
class IDropListener;

class ImageViewer : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageViewer(IDropListener *dropListener, MainWindow *parent);

    virtual ~ImageViewer();
    
    void displayImages(const ImagesPtr images);

    void showImageFromComparator(const QPixmap &image, const QString& description);

    void cleanUp();

    void toggleImage();


    // Implementation of a method to capture the image displayed
    // in QGraphicsView, taking into account the current scale (zoom)
    // and visible area. In other words, everything outside the
    // currently visible area will be cropped.
    SaveImageInfo getCurrentVisiableArea();
    SaveImageInfo getImageShowedOnTheScreen();

    // The RGB values of the pixel under the mouse cursor are displayed
    // in the Color Picker if it is open. If true, we track the color
    // changes under the cursor; if false, we do not.
    void onColorUnderCursorTrackingStatusChanged(bool isActivate);

    // Filters can be applied to modify images. This function removes
    // all filters applied to the compared images.
    void replaceDisplayedImages(const QPixmap &image1, const QPixmap &image2);

    void zoomIn();
    void zoomOut();
    void setToActualSize();
    void setToFitImageInView();

    bool hasActiveSession();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override ;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    MainWindow *parent;
    IDropListener *dropListener;
    QGraphicsScene *customScene;
    QString firstImagePath;
    QString secondImagePath;
    QString firstImageName;
    QString secondImageName;
    QGraphicsPixmapItem *firstDisplayedImage;
    QGraphicsPixmapItem *secondDisplayedImage;
    QGraphicsPixmapItem *comparatorResultDisplayedImage;
    int currentImageIndex;
    qreal scaleFactor;
    bool isColorUnderCursorTrackingActive;
    std::optional<QPoint> lastCursorPos;

    // Zoom to selection
    bool selecting;                         // Whether the user is currently selecting an area
    bool isZoomToSelectionEnabled;
    QPoint selectionStart;                  // Start point of the selection (in view coordinates)
    QRect selectionRect;                    // Rectangle being selected (in view coordinates)

    QPixmap getVisiblePixmap();

    ImagesPtr getCroppedImages(QRectF rect);

    void getPixelColorUnderCursor(std::optional<QPoint> cursorPos);

    void fillPixelColorValues(QString visibleImageName,
                              QColor colorOfVisibleImage,
                              QString hiddenImageName,
                              QColor colorOfHiddenImage
                              );
    void setCenterToViewRectCenter();
};


#endif // IMAGEVIEWER_H
