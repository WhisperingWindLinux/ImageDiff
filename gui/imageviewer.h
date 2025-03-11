#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "imagegeometry.h"

#include <mainwindow.h>
#include <qgraphicsview.h>
#include <qlabel.h>

class ImageViewer : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageViewer(MainWindow *parent = nullptr);
    virtual ~ImageViewer();
    
    void showImagesBeingCompared(QPixmap& image1,
                                 QString path1,
                                 QPixmap& image2,
                                 QString path2,
                                 std::shared_ptr<ImageGeometry> imageGeometry);

    void showComparisonImage(QPixmap &image, QString description);

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

    ImageGeometry getImageGometry();
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

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


    void zoomIn();
    void zoomOut();
    static QPixmap getVisiblePixmap(QGraphicsView* view);
};


#endif // IMAGEVIEWER_H
