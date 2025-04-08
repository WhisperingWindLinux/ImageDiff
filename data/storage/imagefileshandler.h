#ifndef IMAGEFILESHANDLER_H
#define IMAGEFILESHANDLER_H

#include <domain/valueobjects/images.h>
#include <qstring.h>
#include <qurl.h>
#include <domain/valueobjects/savefileinfo.h>

class ImagesRepository;

class ImageFilesHandler {
public:
    ImageFilesHandler() = default;
    ~ImageFilesHandler() = default;

    // In case the operation is canceled by the user, nullptr will be returned;
    // this is not an error indicator {
    ImageHolderPtr openImages();
    ImageHolderPtr openImage();
    ImageHolderPtr openImage(const QString &imagePath);
    ImageHolderPtr openImages(const QList<QUrl> &urls);
    ImageHolderPtr openImages(const QString &firstImagePath, const QString &secondImagePath);
    // }

    // In case the operation is canceled by the user, std::nullopt will be returned;
    // this is not an error indicator {
    std::optional<QString> saveImageAs(const SaveImageInfo &saveImageInfo,
                                       const ImageHolderPtr images
                                       );
    // }

    QString saveImageAsTemporary(const QPixmap &image);

private:
    QPixmap coreOpenImage(const QString &imagePath);
    bool validateFile(const QString &filePath);
    void validateImages(ImageHolderPtr images);
};

#endif // IMAGEFILESHANDLER_H
