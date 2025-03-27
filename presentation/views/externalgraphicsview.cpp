#include "externalgraphicsview.h"

#include <qapplication.h>
#include <qevent.h>
#include <qgraphicsitem.h>

ExternalGraphicsView::ExternalGraphicsView(const QPixmap &image, QWidget *parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    imageItem = new QGraphicsPixmapItem(image);
    scene->addItem(imageItem);

    setDragMode(QGraphicsView::ScrollHandDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QColor backgroundColor = QApplication::palette().color(QPalette::Window);
    setBackgroundBrush(backgroundColor);
}

/* Zoom { */

void ExternalGraphicsView::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0)
        zoomIn();
    else
        zoomOut();
    event->accept();
}

void ExternalGraphicsView::zoomIn() {
    setCenterToViewRectCenter();
    scale(1.25, 1.25);
}

void ExternalGraphicsView::zoomOut() {
    setCenterToViewRectCenter();
    scale(0.8, 0.8);
}

void ExternalGraphicsView::setCenterToViewRectCenter() {
    QRectF viewRect = mapToScene(viewport()->geometry()).boundingRect();
    centerOn(viewRect.center());
}

void ExternalGraphicsView::setToFitImageInView() {
    fitInView(imageItem, Qt::KeepAspectRatio);
    scale(1.0, 1.0);
}
