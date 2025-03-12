#ifndef COMPARABLEIMAGE_H
#define COMPARABLEIMAGE_H

#include <qimage.h>

class ComparableImage {

public:
    ComparableImage(QImage image, QString name);
    ComparableImage(QPixmap image, QString name);
    QImage getImage() const;
    QString getName() const;

    friend class ComparableImageTests;

private:
    QImage image;
    QString name;
};

#endif // COMPARABLEIMAGE_H
