#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <mainwindow.h>
#include <qgraphicsview.h>


class ImageViewer : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageViewer(MainWindow *parent = nullptr);

    void showImagesBeingCompared(QPixmap& image1,
                                 QString path1,
                                 QPixmap& image2,
                                 QString path2);

    void showComparisonImage(QPixmap &image, QString description);

    void toggleImage();

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene *scene;
    QString firstImagePath;
    QString secondImagePath;
    QGraphicsPixmapItem *firstImage = nullptr;
    QGraphicsPixmapItem *secondImage = nullptr;
    QGraphicsPixmapItem *comparisonImage = nullptr;
    int currentImageIndex;
    qreal scaleFactor;
    MainWindow *parent = nullptr;

    void zoomIn();
    void zoomOut();
};


#endif // IMAGEVIEWER_H
