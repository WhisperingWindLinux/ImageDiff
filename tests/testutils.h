#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QUuid>
#include <qdir.h>
#include <qpixmap.h>


class TestUtils {
public:
    static QString savePixmapToTempDir(const QPixmap &pixmap, const QString &fileName) {
        QString tempDir = QDir::tempPath();

        QString filePath = tempDir + QDir::separator() + fileName;

        if (pixmap.save(filePath)) {
            return filePath;
        } else {
            return {};
        }
    }

    static QString getPathInTempDir(QString &fileName) {
        QString tempDir = QDir::tempPath();
        return tempDir + QDir::separator() + fileName;
    }

    static QString generateRandomPngFileNameInTempDir() {
        QDir tempDir = QDir::temp();
        QString uniqueFileName = QUuid::createUuid().toString(QUuid::WithoutBraces);
        uniqueFileName.append(".png");
        QString fullPath = tempDir.filePath(uniqueFileName);
        return fullPath;
    }

    static bool deleteFile(const QString &filePath) {
        QFile file(filePath);
        if (!file.exists()) {
            return false;
        }
        if (file.remove()) {
            return true;
        } else {
            return false;
        }
    }
};

#endif // TESTUTILS_H
