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
    QString openImagesFilter;
    QString saveImagesFilter;
    const QString reportFilter = "Report Files (*.html)";
    const QString videoFilter = "Video Files (*.mp4 *.avi *.mkv *.mov *.wmv *.flv *.webm)";

    enum class PathType { Image, Report, Video };

    std::optional<QString> getUserSaveFilePath(const QString &path, PathType pathType);

    OptionalStringPair getUserOpenTwoFilePaths(const QString &baseDir,
                                               PathType pathType
                                               );

    void showWarningMessage(const QString &msg);

    bool validateVideoFile(const QString &path);

};

#endif // FILEDIALOGHANDLER_H
