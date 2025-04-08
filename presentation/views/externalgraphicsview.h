#ifndef EXTERNALGRAPHICSVIEW_H
#define EXTERNALGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QObject>

class ExternalGraphicsView : public QGraphicsView {

    Q_OBJECT

public:
    ExternalGraphicsView(const QPixmap &image, QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;

public slots:
    void zoomIn();
    void zoomOut();
    void setToFitImageInView();

private:
    QGraphicsScene *mScene;
    QGraphicsPixmapItem *mImageItem;

    void setCenterToViewRectCenter();
};

#endif // EXTERNALGRAPHICSVIEW_H
