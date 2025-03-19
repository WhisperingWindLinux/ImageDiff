#ifndef IMAGEVIEWSTATE_H
#define IMAGEVIEWSTATE_H

#include <QRectF>


struct ImageViewState {

  ImageViewState(QRectF rect,
                   double scaleFactor,
                   int imageIndex,
                   bool isRgbTrackingActive,
                   std::optional<QPoint> lastCursorPos)
        : rect(rect),
        scaleFactor(scaleFactor),
        imageIndex(imageIndex),
        isRgbTrackingActive(isRgbTrackingActive),
        lastCursorPos(lastCursorPos) {}

  ImageViewState(const ImageViewState &imageGeometry)
      : rect(imageGeometry.rect),
      scaleFactor(imageGeometry.scaleFactor),
      imageIndex(imageGeometry.imageIndex),
      isRgbTrackingActive(imageGeometry.isRgbTrackingActive),
      lastCursorPos(imageGeometry.lastCursorPos) {}

  ImageViewState()
      : scaleFactor(0),
      imageIndex(0),
      isRgbTrackingActive(false),
      lastCursorPos(std::nullopt) {}

  const QRectF rect;
  const double scaleFactor;
  int imageIndex;
  bool isRgbTrackingActive;
  std::optional<QPoint> lastCursorPos;
};

#endif // IMAGEVIEWSTATE_H
