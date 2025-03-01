#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <MainWindow.h>
#include <qgraphicsview.h>


class ImageViewer : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageViewer(MainWindow *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem1 = nullptr;
    QGraphicsPixmapItem *pixmapItem2 = nullptr;
    QPixmap images[2];
    int currentImageIndex;
    qreal scaleFactor;
    QString file1;
    QString file2;
    MainWindow *parent = nullptr;


    void loadImages();
    void toggleImage();
    void zoomIn();
    void zoomOut();
};


#endif // IMAGEVIEWER_H
