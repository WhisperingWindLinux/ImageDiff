#ifndef MAINWINDOWCALLBACKS_H
#define MAINWINDOWCALLBACKS_H

#include <qpixmap.h>

enum class RgbChannels { All, R, G, B };

struct ImageGeometry {

  ImageGeometry(QRectF rect, double scaleFactor)
      : rect(rect), scaleFactor(scaleFactor) {}

  ImageGeometry()
      : rect(QRectF()), scaleFactor(0) {}

  const QRectF rect;
  const double scaleFactor;

};

class AMainWindowCallbacks {
public:
    virtual void onImagesBeingComparedLoaded(QPixmap &image1,
                                             QString path1,
                                             QPixmap &image2,
                                             QString path2,
                                             bool useCustomImagesGeometry,
                                             ImageGeometry imageGeometry) = 0;

    virtual void onComparisonImagesLoaded(QPixmap &image,
                                          QString description) = 0;

    virtual void onComparisonTextLoaded(QString text) = 0;

    virtual void saveImageAs(QPixmap &image, QString defaultPath) = 0;

    virtual void onRgbValueUnderCursonChanged(QString imageName, int r, int g, int b) = 0;
};


#endif // MAINWINDOWCALLBACKS_H
