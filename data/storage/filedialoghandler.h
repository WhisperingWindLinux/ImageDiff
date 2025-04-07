#ifndef FILEDIALOGHANDLER_H
#define FILEDIALOGHANDLER_H

#include <qpixmap.h>
#include <qstring.h>

typedef std::optional<QPair<QString, QString>> OptionalStringPair;

class FileDialogHandler
{
public:
    FileDialogHandler();
    ~FileDialogHandler() = default;

    std::optional<QString> getUserSaveImagePath(const QString &path);
    std::optional<QString> getUserSaveReportPath(const QString &path);
    OptionalStringPair getUserOpenTwoImagePaths(const QString &baseDir);
    std::optional<QString> getUserOpenImagePath(const QString &baseDir);
    OptionalStringPair getUserOpenTwoVideoPaths(const QString &baseDir);

private:
    enum class PathType { Image, Report, Video };

    QString mOpenImagesFilter;
    QString mSaveImagesFilter;
    const QString mReportFilter = "Report Files (*.html)";
    const QString mVideoFilter = "Video Files (*.mp4 *.avi *.mkv *.mov *.wmv *.flv *.webm)";

    std::optional<QString> getUserSaveFilePath(const QString &path, PathType pathType);

    OptionalStringPair getUserOpenTwoFilePaths(const QString &baseDir,
                                               PathType pathType
                                               );

    void showWarningMessage(const QString &msg);

    bool validateVideoFile(const QString &path);
};

#endif // FILEDIALOGHANDLER_H
