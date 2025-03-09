#ifndef IMAGECOMPARATOR_H
#define IMAGECOMPARATOR_H

#include <qimage.h>
#include <qstring.h>


class ImageComparator
{
public:
    ImageComparator(const QString &image1Path, const QString &image2Pat);
    QString compareImagesAndSaveResultAsImage();

private:
    QString image1Path;
    QString image2Path;

    QString saveResultImage(const QImage &resultImg);
};

#endif // IMAGECOMPARATOR_H
