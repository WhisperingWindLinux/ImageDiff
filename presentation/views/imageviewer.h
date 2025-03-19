#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <domain/valueobjects/imageviewstate.h>
#include <domain/valueobjects/savefileinfo.h>
#include <qgraphicsview.h>
#include <qlabel.h>

class MainWindow;

class ImageViewer : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageViewer(MainWindow *parent = nullptr);
    virtual ~ImageViewer();
    
    void showTwoImagesBeingCompared(QPixmap& image1,
                                 QString path1,
                                 QPixmap& image2,
                                 QString path2,
                                 std::shared_ptr<ImageViewState> imageGeometry);

    void showImageFromComparator(QPixmap &image, QString description);

    void toggleImage();

    /*
     Implementation of a method to capture the image displayed
     in QGraphicsView, taking into account the current scale (zoom)
     and visible area. In other words, everything outside the
     currently visible area will be cropped.
    */
    SaveImageInfo getCurrentVisiableArea();

    SaveImageInfo getImageShowedOnTheScreen();

    void onColorPickerStatusChanged(bool isActivate);

    ImageViewState getCurrentState();

    void zoomIn();
    void zoomOut();
    void actualSize();
    void fitImageInView();
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override ;
    void mouseReleaseEvent(QMouseEvent *event) override ;
    void paintEvent(QPaintEvent *event) override;

private:
    QGraphicsScene *scene;
    QString firstImagePath;
    QString secondImagePath;
    QString firstImageName;
    QString secondImageName;
    QGraphicsPixmapItem *firstImage = nullptr;
    QGraphicsPixmapItem *secondImage = nullptr;
    QGraphicsPixmapItem *comparatorResultImage = nullptr;
    int currentImageIndex;
    qreal scaleFactor;
    MainWindow *parent = nullptr;
    std::optional<QPoint> lastCursorPos = std::nullopt;
    bool isRgbTrackingActive = false;

    // Zoom to selection
    bool selecting;                         // Whether the user is currently selecting an area
    bool isZoomToSelectionEnabled = false;
    QPoint selectionStart;                  // Start point of the selection (in view coordinates)
    QRect selectionRect;                    // Rectangle being selected (in view coordinates)

    static QPixmap getVisiblePixmap(QGraphicsView* view);

    void passCropedImageToOtherAppInstance(QRectF rect);

    void trackPixelColor(std::optional<QPoint> cursorPos);

    void fillRgbValues(QString visibleImageName,
                                       QColor colorOfVisibleImage,
                                       QString hiddenImageName,
                                       QColor colorOfHiddenImage
                                       );
};


#endif // IMAGEVIEWER_H
