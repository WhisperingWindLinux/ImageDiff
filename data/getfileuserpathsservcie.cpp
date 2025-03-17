#include "getfileuserpathsservcie.h"

#include <QMimeDatabase>
#include <qfiledialog.h>
#include <qmediaplayer.h>
#include <qmessagebox.h>

GetFileUserPathsService::GetFileUserPathsService() {

}

optional<QString> GetFileUserPathsService::getUserSaveImagePath(QString path) {
    return getUserSaveFilePath(path, PathType::Image);
}

optional<QString> GetFileUserPathsService::getUserSaveReportPath(QString path) {
    return getUserSaveFilePath(path, PathType::Report);
}

optional<QString> GetFileUserPathsService::getUserSaveFilePath(QString path,
                                                               PathType savedFileType
                                                               )
{
    QFileDialog dialog;
    QString filePath;
    QString filter;
    QString title;

    if (savedFileType == PathType::Image) {
        filter = imageFilter;
        title = "Save Image";
    } else if (savedFileType == PathType::Report) {
        filter = reportFilter;
        title = "Save Report";
    } else {
        throw runtime_error("Error: The app is trying to save a file of an unsupported type."
                            " This error message is for the app developer.");
    }

    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setWindowTitle(title);
    dialog.setNameFilter(filter);
    dialog.setModal(true);

    if (path.isEmpty()) {
        dialog.setDirectory(QDir::homePath());
    } else {
        dialog.setDirectory(path);
    }

    if (dialog.exec() == QDialog::Accepted) {
        QStringList fileNames = dialog.selectedFiles();
        if (fileNames.size() == 0) {
            showWarningMessage("You have not selected any path to save the file.");
            return nullopt;
        }
        else if (fileNames.size() == 1) {
            filePath = dialog.selectedFiles().constFirst();
        }
    } else {
        return nullopt;
    }

    if (filePath.isEmpty()) {
        return nullopt;
    }

    return make_optional<QString>(filePath);
}

std::optional<QString> GetFileUserPathsService::getUserOpenImagePath(QString baseDir)
{
    QFileDialog dialog;
    QString firstPath;    
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setWindowTitle("Open Image");
    dialog.setNameFilter(imageFilter);
    dialog.setModal(true);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (baseDir.isEmpty()) {
        dialog.setDirectory(QDir::homePath());
    } else {
        dialog.setDirectory(baseDir);
    }

    if (dialog.exec() == QDialog::Accepted) {
        QStringList fileNames = dialog.selectedFiles();
        if (fileNames.size() == 0) {
            showWarningMessage("You have not selected any image file.");
            return std::nullopt;
        }
        else if (fileNames.size() == 1) {
            firstPath = dialog.selectedFiles().constFirst();
        }
    } else {
        return std::nullopt;
    }
    if (firstPath.isEmpty()) {
        return nullopt;
    }

    return make_optional<QString>(firstPath);
}

OptionalPathPair GetFileUserPathsService::getUserOpenTwoImagePaths(QString baseDir) {
    return getUserOpenTwoFilePaths(baseDir, PathType::Image);
}

OptionalPathPair GetFileUserPathsService::getUserOpenTwoVideoPaths(QString baseDir) {
    return getUserOpenTwoFilePaths(baseDir, PathType::Video);
}

OptionalPathPair GetFileUserPathsService::getUserOpenTwoFilePaths(QString baseDir, PathType pathType) {
    QFileDialog dialog;
    QString firstFile;
    QString secondFile;

    QString filter;
    QString title1;
    QString title2;

    if (pathType == PathType::Image) {
        filter = imageFilter;
        title1 = "Open First Image";
        title2 = "Open Second Image";
    } else if (pathType == PathType::Video) {
        filter = videoFilter;
        title1 = "Open First Video";
        title2 = "Open Second Video";
    } else {
        throw runtime_error("Error: The app is trying to open a file of an unsupported type."
                            " This error message is for the app developer.");
    }

    dialog.setViewMode(QFileDialog::Detail);
    dialog.setWindowTitle(title1);
    dialog.setNameFilter(filter);
    dialog.setModal(true);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (baseDir.isEmpty()) {
        dialog.setDirectory(QDir::homePath());
    } else {
        dialog.setDirectory(baseDir);
    }

    if (dialog.exec() == QDialog::Accepted) {
        QStringList fileNames = dialog.selectedFiles();
        if (fileNames.size() == 0) {
            showWarningMessage("You have not selected any file.");
            return std::nullopt;
        }
        else if (fileNames.size() == 1) {
            firstFile = dialog.selectedFiles().constFirst();
        }
    } else {
        return std::nullopt;
    }

    dialog.setWindowTitle(title2);

    if (dialog.exec() == QDialog::Accepted) {
        QStringList fileNames = dialog.selectedFiles();
        if (fileNames.size() == 0) {
            showWarningMessage("You have not selected any file.");
            return std::nullopt;
        }
        else if (fileNames.size() == 1) {
            secondFile = dialog.selectedFiles().constFirst();
        }
    } else {
        return std::nullopt;
    }

    if (firstFile.isEmpty() || secondFile.isEmpty()) {
        return nullopt;
    }

    if (pathType == PathType::Video) {
        bool isValid = validateVideoFile(firstFile);
        isValid &= validateVideoFile(secondFile);
        if (!isValid) return nullopt;
    }

    return std::make_optional<QPair<QString, QString>>(firstFile, secondFile);
}

void GetFileUserPathsService::showWarningMessage(QString msg) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

bool GetFileUserPathsService::validateVideoFile(QString path) {
    if (path.isEmpty()) {
        return false;
    }
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(path);

    if (mimeType.name().startsWith("video/")) {
        return true;
    }
    return false;
}
