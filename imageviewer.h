#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <MainWindow.h>
#include <qgraphicsview.h>

#include <interactors/imageviewinteractor.h>


class ImageViewer : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageViewer(MainWindow *parent = nullptr);
    virtual ~ImageViewer();

    bool loadImages();
    void toggleImage();
    void showDifferenceAsImage();
    void showPixelsBrigthnessDifference();
    void showPixeslContrastDifference();
    void showPixelsSaturationDifference();
    void showAbsolutePixelsValueDifference();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *firstImagePixmap = nullptr;
    QGraphicsPixmapItem *secondImagePixmap = nullptr;
    QGraphicsPixmapItem *comparisonImagePixmap = nullptr;
    int currentImageIndex;
    bool isComparisonImageShowing;
    qreal scaleFactor;
    MainWindow *parent = nullptr;
    ImageViewInteractor *imageViewInteractor = nullptr;

    void zoomIn();
    void zoomOut();
    void showError(const QString &errorMessage);
};


#endif // IMAGEVIEWER_H
