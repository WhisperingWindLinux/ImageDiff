#ifndef IMAGESREPOSITORY_H
#define IMAGESREPOSITORY_H

#include <qpixmap.h>
#include <domain/interfaces/imagerepository.h>

class ImageFilesHandler;

// It stores two compared images and one image as the result of their
// comparison (the result of the comparator's work), as well as some
// metadata about the images, such as paths, names, etc.

class ImagesRepository : public IImagesRepository
{
public:
    ImagesRepository(const QString &image1Path,
                     const QString &image2Path,
                     const QPixmap &image1,
                     const QPixmap &image2
                     );

    virtual ~ImagesRepository();

    void update(const QPixmap &image1, const QPixmap &image2) override;
    void update(const QPixmap &comparisonResultImage) override;
    void setDeleteFilesAfterClose();
    void restoreOriginalImages();

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

typedef std::shared_ptr<ImagesRepository> ImagesRepositoryPtr;

#endif // IMAGESREPOSITORY_H
