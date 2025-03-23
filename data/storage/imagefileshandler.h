#ifndef IMAGEFILESHANDLER_H
#define IMAGEFILESHANDLER_H

#include <domain/valueobjects/images.h>
#include <qstring.h>
#include <qurl.h>
#include <domain/valueobjects/savefileinfo.h>

class ImagesRepository;

struct FileSaveResult {
  FileSaveResult(bool isSaved, const QString &path): isSaved(isSaved), path(path) {}
  const bool isSaved;
  const QString path;
};

class ImageFilesHandler
{
public:
    ImageFilesHandler() = default;
    ~ImageFilesHandler() = default;

    // The openImages functions never returns nullptr; if an error occurs,
    // a runtime_error with its description will be thrown.
    ImagesPtr openImages();
    ImagesPtr openImages(const QString &image1Path, const QString &image2Path);
    ImagesPtr openImages(const QList<QUrl> &urls);

    std::optional<FileSaveResult> saveImageAs(const SaveImageInfo &saveImageInfo, const ImagesPtr images);

private:
    bool validateFile(const QString &filePath);
    void validateImages(ImagesPtr images);
};

#endif // IMAGEFILESHANDLER_H
