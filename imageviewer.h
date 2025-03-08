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
    QGraphicsPixmapItem *pixmapItem3ComparisonResult = nullptr;
    QPixmap images[3];
    int currentImageIndex;
    bool isCompareResultImageShowing;
    qreal scaleFactor;
    QString file1;
    QString file2;
    QString file3ComparisonResult;
    MainWindow *parent = nullptr;


    void loadImages();
    void toggleImage();
    void showCompareResultImage();
    void zoomIn();
    void zoomOut();
    void showCalculatedImageDiff();
    bool validateFilePath(const QString &filePath);
    void showErrorAndExit(const QString &errorMessage);
};


#endif // IMAGEVIEWER_H
