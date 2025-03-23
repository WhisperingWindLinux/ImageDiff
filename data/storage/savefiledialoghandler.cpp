#include "savefiledialoghandler.h"

#include <QMimeDatabase>
#include <qfiledialog.h>
#include <qmediaplayer.h>
#include <qmessagebox.h>

std::optional<QString> SaveFileDialogHandler::getUserSaveImagePath(const QString &path) {
    return getUserSaveFilePath(path, PathType::Image);
}

std::optional<QString> SaveFileDialogHandler::getUserSaveReportPath(const QString &path) {
    return getUserSaveFilePath(path, PathType::Report);
}

std::optional<QString> SaveFileDialogHandler::getUserSaveFilePath(const QString &path,
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
        throw std::runtime_error("An internal error occurred: the app is trying to save a file of an unsupported type.");
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
            return std::nullopt;
        }
        else if (fileNames.size() == 1) {
            filePath = dialog.selectedFiles().constFirst();
        }
    } else {
        return std::nullopt;
    }

    if (filePath.isEmpty()) {
        return std::nullopt;
    }

    return std::make_optional<QString>(filePath);
}

std::optional<QString> SaveFileDialogHandler::getUserOpenImagePath(const QString &baseDir) {
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
        return std::nullopt;
    }

    return std::make_optional<QString>(firstPath);
}

OptionalStringPair SaveFileDialogHandler::getUserOpenTwoImagePaths(const QString &baseDir) {
    return getUserOpenTwoFilePaths(baseDir, PathType::Image);
}

OptionalStringPair SaveFileDialogHandler::getUserOpenTwoVideoPaths(const QString &baseDir) {
    return getUserOpenTwoFilePaths(baseDir, PathType::Video);
}

OptionalStringPair SaveFileDialogHandler::getUserOpenTwoFilePaths(const QString &baseDir,
                                                                  PathType pathType
                                                                  )
{
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
        QString err = QString("An internal error occurred: the app is trying to open a file of an unsupported type.")
                      + " This error message is for the app developer.";
        throw std::runtime_error(err.toStdString());
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
        return std::nullopt;
    }

    if (pathType == PathType::Video) {
        bool isValid = validateVideoFile(firstFile);
        isValid &= validateVideoFile(secondFile);
        if (!isValid) return std::nullopt;
    }

    return std::make_optional<QPair<QString, QString>>(firstFile, secondFile);
}

void SaveFileDialogHandler::showWarningMessage(const QString &msg) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

bool SaveFileDialogHandler::validateVideoFile(const QString &path) {
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
