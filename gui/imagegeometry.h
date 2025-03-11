#ifndef IMAGEGEOMETRY_H
#define IMAGEGEOMETRY_H

#include <QRectF>


struct ImageGeometry {

  ImageGeometry(QRectF rect, double scaleFactor)
      : rect(rect), scaleFactor(scaleFactor) {}

  ImageGeometry(const ImageGeometry &imageGeometry)
      : rect(imageGeometry.rect)
      , scaleFactor(imageGeometry.scaleFactor) {}

  ImageGeometry()
      : rect(QRectF()), scaleFactor(0) {}

  const QRectF rect;
  const double scaleFactor;
};

#endif // IMAGEGEOMETRY_H
