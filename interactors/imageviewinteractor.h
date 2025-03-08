#ifndef IMAGEVIEWINTERACTOR_H
#define IMAGEVIEWINTERACTOR_H

#include <QString>
#include <qgraphicsitem.h>
#include <qpixmap.h>

class ImageViewInteractor
{
public:
    ImageViewInteractor(QString &img1Path, QString &img2Path);
    void loadImages();

    QString getFirstImagePath() const;
    QString getSecondImagePath() const;
    QString getComparisonImagePath() const;
    QPixmap getFirstImagePixmap() const;
    QPixmap getSecondImagePixmap() const;
    QPixmap getComparisonImagePixmap() const;

private:
    QString firstImagePath;
    QString secondImagePath;
    QString comparisonImagePath;
    QList<QPixmap> images {3};

    bool validateFilePath(const QString &filePath);
    QPixmap getImagePixmap(int index) const;
};

#endif // IMAGEVIEWINTERACTOR_H
