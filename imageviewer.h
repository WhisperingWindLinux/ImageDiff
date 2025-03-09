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

    void loadImages();
    void toggleImage();
    void showCompareResultImage();
    void showPexelsBrightnessDiff();
    void showContrastDiff();
    void showColorsSaturationDiff();
    void zoomIn();
    void zoomOut();
    void showCalculatedImageDiff();
    void showError(const QString &errorMessage);
};


#endif // IMAGEVIEWER_H
