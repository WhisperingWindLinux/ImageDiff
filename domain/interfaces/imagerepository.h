#ifndef IIMAGEREPOSITORY_H
#define IIMAGEREPOSITORY_H

#include <qpixmap.h>
#include <qstring.h>

class IImagesRepository
{
public:
    virtual void update(const QPixmap &image1, const QPixmap &image2) = 0;

    virtual void update(const QPixmap &comparisonResultImage) = 0;

    virtual const QString& getFirstImagePath() const = 0;

    virtual const QString& getSecondImagePath() const = 0;

    virtual const QPixmap& getFirstImage() const = 0;

    virtual const QPixmap& getSecondImage() const = 0;

    virtual const QPixmap& getComparisonResultImage() const = 0;

    virtual const QString& getFirstImageBaseName() const = 0;

    virtual const QString& getSecondImageBaseName() const = 0;

    virtual const QString& getFirstImageDir() const = 0;

    virtual const QString& getSecondImageDir() const = 0;
};

typedef std::shared_ptr<IImagesRepository> IImagesRepositoryPtr;

#endif // IIMAGEREPOSITORY_H
