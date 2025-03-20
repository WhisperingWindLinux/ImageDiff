#ifndef SAVEFILEDIALOGHANDLER_H
#define SAVEFILEDIALOGHANDLER_H

#include <qpixmap.h>
#include <qstring.h>

using namespace std;

typedef optional<QPair<QString, QString>> OptionalPathPair;



class SaveFileDialogHandler
{
public:
    SaveFileDialogHandler();

    optional<QString> getUserSaveImagePath(QString path);

    optional<QString> getUserSaveReportPath(QString path);

    OptionalPathPair getUserOpenTwoImagePaths(QString baseDir);

    optional<QString> getUserOpenImagePath(QString baseDir);

    OptionalPathPair getUserOpenTwoVideoPaths(QString baseDir);

private:
    const QString reportFilter = "Report Files (*.html)";
    const QString imageFilter = "Image Files (*.png)";
    const QString videoFilter = "Video Files (*.mp4 *.avi *.mkv *.mov *.wmv *.flv *.webm)";

    enum class PathType { Image, Report, Video };

    optional<QString> getUserSaveFilePath(QString path, PathType pathType);
    OptionalPathPair getUserOpenTwoFilePaths(QString baseDir, PathType pathType);
    void showWarningMessage(QString msg);

    bool validateVideoFile(QString path);

};

#endif // SAVEFILEDIALOGHANDLER_H
