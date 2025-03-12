#ifndef IMAGEGEOMETRY_H
#define IMAGEGEOMETRY_H

#include <QRectF>


struct ImageGeometry {

  ImageGeometry(QRectF rect, double scaleFactor, int imageIndex)
        : rect(rect), scaleFactor(scaleFactor), imageIndex(imageIndex) {}

  ImageGeometry(const ImageGeometry &imageGeometry)
      : rect(imageGeometry.rect),
        scaleFactor(imageGeometry.scaleFactor),
      imageIndex(imageGeometry.imageIndex)  {}

  ImageGeometry()
      : rect(QRectF()), scaleFactor(0), imageIndex(0) {}

  const QRectF rect;
  const double scaleFactor;
  int imageIndex;
};

#endif // IMAGEGEOMETRY_H
