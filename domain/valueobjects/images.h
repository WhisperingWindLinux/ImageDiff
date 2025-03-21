#ifndef IMAGES_H
#define IMAGES_H

#include <qpixmap.h>


struct Images {
  Images(QPixmap image1, QPixmap image2, QString path1, QString path2)
      : image1(std::move(image1)), image2(std::move(image2)),
        path1(std::move(path1)), path2(std::move(path2)) {}
  const QPixmap image1;
  const QPixmap image2;
  const QString path1;
  const QString path2;
};

typedef std::shared_ptr<Images> ImagesPtr;

#endif // IMAGES_H
