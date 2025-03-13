#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "imageviewstate.h"

#include <mainwindow.h>
#include <qgraphicsview.h>
#include <qlabel.h>

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
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override ;
    void mouseReleaseEvent(QMouseEvent *event) override ;
    void paintEvent(QPaintEvent *event) override;
    /*void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;*/

private:
    QGraphicsScene *scene;
    QString firstImagePath;
    QString secondImagePath;
    QString firstImageName;
    QString secondImageName;
    QGraphicsPixmapItem *firstImage = nullptr;
    QGraphicsPixmapItem *secondImage = nullptr;
    QGraphicsPixmapItem *comparisonImage = nullptr;
    int currentImageIndex;
    qreal scaleFactor;
    MainWindow *parent = nullptr;
    QMouseEvent* lastMousEvent = nullptr;
    bool isRgbTrackingActive = false;

    // Zoom to selection
    bool selecting;                         // Whether the user is currently selecting an area
    bool isZoomToSelectionEnabled = false;
    QPoint selectionStart;                  // Start point of the selection (in view coordinates)
    QRect selectionRect;                    // Rectangle being selected (in view coordinates)

    static QPixmap getVisiblePixmap(QGraphicsView* view);
};


#endif // IMAGEVIEWER_H
