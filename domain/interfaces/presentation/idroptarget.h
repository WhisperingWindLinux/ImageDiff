#ifndef IDROPTARGET_H
#define IDROPTARGET_H

#include <QtCore/qurl.h>
class IDropListener {
public:
    virtual void onDrop(QList<QUrl> urls) = 0;
};

#endif // IDROPTARGET_H
