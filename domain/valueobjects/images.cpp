#include "images.h"

#ifdef QT_DEBUG
    int Images::generation = 0;
#endif

Images::Images(const QPixmap &image1, const QPixmap &image2, const QString &path1, const QString &path2)
        : image1(image1),
        image2(image2),
        path1(path1),
        path2(path2),
        isTemporaryFiles(false)
{
    #ifdef QT_DEBUG
        currentGeneration = ++generation;
        qDebug() << "Images { Generation " << currentGeneration << "created! }";
    #endif
}

Images::~Images() {
    #ifdef QT_DEBUG
        qDebug() << "Images { Generation " << currentGeneration << "killed! }";
    #endif
    if (!isTemporaryFiles) {
        return;
    }
    QFile(path1).remove();
    QFile(path2).remove();
}

void Images::markAsTemporary() {
    isTemporaryFiles = true;
}

bool Images::getIsTemporaryFiles() const {
    return isTemporaryFiles;
}

bool Images::isTheSameImage() const {
    return path1 == path2;
}
