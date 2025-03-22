#ifndef COMPARABLEIMAGE_H
#define COMPARABLEIMAGE_H

#include <qimage.h>

class ComparableImage {

public:
    ComparableImage(const QImage &image, const QString &name);
    ComparableImage(QImage &&image, QString &&name);

    ComparableImage(const QPixmap &image, const QString &name);

    const QImage& getImage() const;
    const QString& getName() const;

    friend class ComparableImageTests;

private:
    QImage image;
    QString name;
};

#endif // COMPARABLEIMAGE_H
