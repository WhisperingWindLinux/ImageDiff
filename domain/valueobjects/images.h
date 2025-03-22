#ifndef IMAGES_H
#define IMAGES_H

#include <QtCore/qdebug.h>
#include <qfile.h>
#include <qpixmap.h>

struct Images {

Images(const QPixmap &image1, const QPixmap &image2, const QString &path1, const QString &path2);

    ~Images();

    // When the user selects an area of the image with the mouse while holding
    // down Command, a new instance of the application opens with cropped images.
    // These images are saved in the Temp folder and will be deleted upon closing
    // the application unless the user saves them.
    void markAsTemporary();

    bool getIsTemporaryFiles() const;

    QPixmap image1;
    QPixmap image2;
    QString path1;

    QString path2;

private:
    bool isTemporaryFiles;
    #ifdef QT_DEBUG
        static int generation;
        int currentGeneration;
    #endif
};

typedef std::shared_ptr<Images> ImagesPtr;

#endif // IMAGES_H
