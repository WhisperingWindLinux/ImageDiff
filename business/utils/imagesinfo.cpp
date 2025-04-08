#include "imagesinfo.h"

ImagesInfo::ImagesInfo(const ImageHolderPtr images) {
    if (images == nullptr) {
        return;
    }
    QFileInfo firstImageInfo{images->getFirstImagePath()};
    mFirstImagePath = firstImageInfo.absoluteFilePath();
    mFirstImageName = firstImageInfo.fileName();
    mFirstImageBaseName = firstImageInfo.baseName();
    mFirstImageDir = firstImageInfo.dir().absolutePath();

    if (images->isPairOfImages()) {
        QFileInfo secondImageInfo{images->getSecondImagePath()};
        mSecondImagePath = secondImageInfo.absoluteFilePath();
        mSecondImageName = secondImageInfo.fileName();
        mSecondImageBaseName = secondImageInfo.baseName();
        mSecondImageDir = secondImageInfo.dir().absolutePath();
    }
}

QString ImagesInfo::getFirstImagePath() const {
    return mFirstImagePath;
}

QString ImagesInfo::getSecondImagePath() const {
    return mSecondImagePath;
}

QString ImagesInfo::getFirstImageName() const {
    return mFirstImageName;
}

QString ImagesInfo::getSecondImageName() const {
    return mSecondImageName;
}

QString ImagesInfo::getFirstImageBaseName() const {
    return mFirstImageBaseName;
}

QString ImagesInfo::getSecondImageBaseName() const {
    return mSecondImageBaseName;
}

QString ImagesInfo::getFirstImageDir() const {
    return mFirstImageDir;
}

QString ImagesInfo::getSecondImageDir() const {
    return mSecondImageDir;
}
