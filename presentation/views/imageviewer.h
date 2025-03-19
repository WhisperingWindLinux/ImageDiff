#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <domain/valueobjects/savefileinfo.h>
#include <qgraphicsview.h>

class MainWindow;

class ImageViewer : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageViewer(MainWindow *parent = nullptr);
    virtual ~ImageViewer();
    
    void displayImages(QPixmap& image1,
                                    QString path,
                                    QPixmap& image2,
                                    QString path2
                                    );

    void showImageFromComparator(QPixmap &image, QString description);

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
    void replaceDisplayedImages(QPixmap &pixmap1, QPixmap &pixmap2);

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

private:
    MainWindow *parent;
    QGraphicsScene *scene;
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

    static QPixmap getVisiblePixmap(QGraphicsView* view);

    void passCropedImageToOtherAppInstance(QRectF rect);

    void getPixelColorUnderCursor(std::optional<QPoint> cursorPos);

    void fillPixelColorValues(QString visibleImageName,
                              QColor colorOfVisibleImage,
                              QString hiddenImageName,
                              QColor colorOfHiddenImage
                              );
    void setCenterToViewRectCenter();
};


#endif // IMAGEVIEWER_H
