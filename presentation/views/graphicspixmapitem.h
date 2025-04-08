#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

class IDropListener;

// Support for opening files via drop in QGraphicsView

class GraphicsPixmapItem : public QGraphicsPixmapItem {
public:
    GraphicsPixmapItem(const QPixmap &pixmap,
                       IDropListener *dropListener,
                       QGraphicsItem *parent = nullptr
                       );
    virtual ~GraphicsPixmapItem();

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    IDropListener *mDropListener;
};

#endif // GRAPHICSPIXMAPITEM_H
