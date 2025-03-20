#ifndef IMAGESREPOSITORY_H
#define IMAGESREPOSITORY_H

#include <qpixmap.h>
#include <domain/interfaces/imagerepository.h>


class ImagesRepository : public IImagesRepository
{
public:
    ImagesRepository();

    bool update(const QString &image1Path, const QString &image2Path, bool isTemporaryFiles = false);
    void update(const QPixmap &image1, const QPixmap &image2);
    void update(const QPixmap &comparisonResultImage);
    void clear();

    void restoreOriginalImages();

    const QString& getDefaultImageExtention(bool includeDot = false) const;

    // IImageRepository interface

    const QString& getFirstImagePath() const override;
    const QString& getSecondImagePath() const override;
    const QPixmap& getFirstImage() const override;
    const QPixmap& getSecondImage() const override;
    const QPixmap& getComparisonResultImage() const override;
    const QString& getFirstImageBaseName() const override;
    const QString& getSecondImageBaseName() const override;
    const QString& getFirstImageDir() const override;
    const QString& getSecondImageDir() const override;

private:
    QString imageExtentionWithoutDot;
    QString imageExtentionWithDot;
    bool isTemporaryFiles;
    QString firstImagePath;
    QString secondImagePath;
    QString firstImageName;
    QString secondImageName;
    QString firstImageBaseName;
    QString secondImageBaseName;
    QString firstImageDir;
    QString secondImageDir;
    QPixmap originalFirstImage;
    QPixmap originalSecondImage;
    QPixmap firstCurrentlyDisplayedImage;
    QPixmap secondCurrentlyDisplayedImage;
    QPixmap displayedComparisonResultImage;
};

#endif // IMAGESREPOSITORY_H
