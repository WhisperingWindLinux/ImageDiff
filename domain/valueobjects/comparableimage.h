#ifndef COMPARABLEIMAGE_H
#define COMPARABLEIMAGE_H

#include <qimage.h>

class ComparableImage {
public:
    ComparableImage(const QImage &image, const QString &imageName);
    ComparableImage(QImage &&image, QString &&imageName);

    ComparableImage(const QPixmap &image, const QString &imageName);

    QImage getImage() const;

    QString getImageName() const;
    QString getPath() const;

private:
    QImage mImage;
    QString mImageName;
};

#endif // COMPARABLEIMAGE_H
