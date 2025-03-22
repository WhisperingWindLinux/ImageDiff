#include "images.h"

#ifdef QT_DEBUG
    int Images::generation = 0;
#endif

Images::Images(const QPixmap &image1, const QPixmap &image2, const QString &path1, const QString &path2)
    : image1(std::move(image1)), image2(std::move(image2)),
    path1(std::move(path1)), path2(std::move(path2)), isTemporaryFiles(false)
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
