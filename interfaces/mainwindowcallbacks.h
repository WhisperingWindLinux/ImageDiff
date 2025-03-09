#ifndef MAINWINDOWCALLBACKS_H
#define MAINWINDOWCALLBACKS_H

#include <qpixmap.h>


class AMainWindowCallbacks {
public:
    virtual void onImagesBeingComparedLoaded(QPixmap &image1,
                                             QString path1,
                                             QPixmap &image2,
                                             QString path2) = 0;

    virtual void onComparisonImagesLoaded(QPixmap &image,
                                          QString description) = 0;

    virtual void onComparisonTextLoaded(QString text) = 0;
};


#endif // MAINWINDOWCALLBACKS_H
