#include "graphicspixmapitem.h"

#include <domain/interfaces/IDropTarget.h>


GraphicsPixmapItem::GraphicsPixmapItem(const QPixmap &pixmap,
                                       IDropListener *dropListener,
                                       QGraphicsItem *parent
                                       )
    : QGraphicsPixmapItem(pixmap, parent),
    dropListener(dropListener)
{
    setAcceptDrops(true);
}

GraphicsPixmapItem::~GraphicsPixmapItem() {

}

void GraphicsPixmapItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    if (event->mimeData()->hasFormat("text/uri-list") && dropListener) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void GraphicsPixmapItem::dropEvent(QGraphicsSceneDragDropEvent *event) {
    if (!event->mimeData()->hasUrls() || !dropListener) {
        event->ignore();
    } else {
        event->acceptProposedAction();
        QList<QUrl> urls = event->mimeData()->urls();
        dropListener->onDrop(urls);
    }
}
