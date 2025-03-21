#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QtCore/qdir.h>
#include <domain/valueobjects/images.h>
#include <qfileinfo.h>
#include <qstring.h>


class ImagesInfo {
public:
    ImagesInfo(const ImagesPtr images) {
        if (images == nullptr) {
            return;
        }

        QFileInfo firstImageInfo { images->path1 };
        QFileInfo secondImageInfo { images->path2 };

        firstImagePath = firstImageInfo.absoluteFilePath();
        secondImagePath = secondImageInfo.absoluteFilePath();

        firstImageName = firstImageInfo.fileName();
        secondImageName = secondImageInfo.fileName();
        firstImageBaseName = firstImageInfo.baseName();
        secondImageBaseName = secondImageInfo.baseName();
        firstImageDir = firstImageInfo.dir().absolutePath();
        secondImageDir = secondImageInfo.dir().absolutePath();
    };


    const QString& getFirstImagePath() const {
        return firstImagePath;
    }

    const QString& getSecondImagePath() const {
        return secondImagePath;
    }

    const QString& getFirstImageBaseName() const {
        return firstImageBaseName;
    }

    const QString& getSecondImageBaseName() const {
        return secondImageBaseName;
    }

    const QString& getFirstImageDir() const {
        return firstImageDir;
    }

    const QString& getSecondImageDir() const {
        return secondImageDir;
    }

private:
    QString firstImagePath;
    QString secondImagePath;
    QString firstImageName;
    QString secondImageName;
    QString firstImageBaseName;
    QString secondImageBaseName;
    QString firstImageDir;
    QString secondImageDir;
};


#endif // FILEUTILS_H
