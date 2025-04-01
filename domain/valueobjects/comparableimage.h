#ifndef COMPARABLEIMAGE_H
#define COMPARABLEIMAGE_H

#include <qimage.h>

class ComparableImage {

public:
    ComparableImage(const QImage &image, const QString &name);
    ComparableImage(QImage &&image, QString &&name);

    ComparableImage(const QPixmap &image, const QString &name);

    QImage getImage() const;

    QString getBaseName() const;
    QString getPath() const;

private:
    QImage image;
    QString path;
};

#endif // COMPARABLEIMAGE_H
