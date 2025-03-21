#ifndef IMAGES_H
#define IMAGES_H

#include <qfile.h>
#include <qpixmap.h>

struct Images {

Images(QPixmap image1, QPixmap image2, QString path1, QString path2)
      : image1(std::move(image1)), image2(std::move(image2)),
        path1(std::move(path1)), path2(std::move(path2)), isTemporaryFiles(false) {}

  ~Images() {
      if (!isTemporaryFiles) {
          return;
      }
      QFile(path1).remove();
      QFile(path2).remove();
  }

  // When the user selects an area of the image with the mouse while holding
  // down Command, a new instance of the application opens with cropped images.
  // These images are saved in the Temp folder and will be deleted upon closing
  // the application unless the user saves them.
  void markAsTemporary() {
      isTemporaryFiles = true;
  }

  const QPixmap image1;
  const QPixmap image2;
  const QString path1;
  const QString path2;

  bool getIsTemporaryFiles() const { return isTemporaryFiles; }

private:
  bool isTemporaryFiles;

};



typedef std::shared_ptr<Images> ImagesPtr;

#endif // IMAGES_H
