#ifndef IMAGEFILESHANDLER_H
#define IMAGEFILESHANDLER_H

#include <domain/interfaces/imagerepository.h>
#include <domain/valueobjects/images.h>
#include <qstring.h>
#include <qurl.h>
#include <domain/valueobjects/savefileinfo.h>

class ImagesRepository;

struct FileSaveResult {
  FileSaveResult(bool isSaved, const QString &path): isSaved(isSaved), path(path) {}
  const bool isSaved;
  const QString &path;
};

class ImageFilesHandler
{
public:
    static const QString imageExtentionWithoutDot;
    static const QString imageExtentionWithDot;

    ImageFilesHandler() = default;
    ~ImageFilesHandler() = default;

    ImagesPtr openImages();
    ImagesPtr openImages(const QString &image1Path, const QString &image2Path);
    ImagesPtr openImages(const QList<QUrl> &urls);
    bool validateFile(const QString &filePath);
    FileSaveResult saveImage(const SaveImageInfo &info, const IImagesRepositoryPtr imagerepository);

private:
    static const std::string errorUnableToOpenImages;
};

#endif // IMAGEFILESHANDLER_H
