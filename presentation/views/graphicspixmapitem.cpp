#include "graphicspixmapitem.h"

#include <domain/interfaces/presentation/idroptarget.h>


GraphicsPixmapItem::GraphicsPixmapItem(const QPixmap &pixmap,
                                       IDropListener *dropListener,
                                       QGraphicsItem *parent
                                       )
    : QGraphicsPixmapItem(pixmap, parent),
    mDropListener(dropListener)
{
    setAcceptDrops(true);
}

GraphicsPixmapItem::~GraphicsPixmapItem() {

}

void GraphicsPixmapItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    if (event->mimeData()->hasFormat("text/uri-list") && mDropListener) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void GraphicsPixmapItem::dropEvent(QGraphicsSceneDragDropEvent *event) {
    if (!event->mimeData()->hasUrls() || !mDropListener) {
        event->ignore();
    } else {
        event->acceptProposedAction();
        QList<QUrl> urls = event->mimeData()->urls();
        mDropListener->onDrop(urls);
    }
}
