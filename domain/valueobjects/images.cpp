#include "images.h"
#include <QtCore/qdebug.h>
#include <qfile.h>

#ifdef QT_DEBUG
int ImageHolder::mGeneration = 0;
#endif

ImageHolder::ImageHolder(const QPixmap &image, const QString &imagePath)
    : mIsTemporary(false),
    mFirstImage(image),
    mFirstImagePath(imagePath)
{
#ifdef QT_DEBUG
    mCurrentGeneration = ++mGeneration;
    qDebug() << "Images { Generation " << mCurrentGeneration << "created! }";
#endif
}

ImageHolder::ImageHolder(const QPixmap &firstImage,
                         const QString &firstImagePath,
                         const QPixmap &secondImage,
                         const QString &secondImagePath
                         )
    : mIsTemporary(false),
    mFirstImage(firstImage),
    mFirstImagePath(firstImagePath),
    mSecondImage(secondImage),
    mSecondImagePath(secondImagePath)
{
#ifdef QT_DEBUG
    mCurrentGeneration = ++mGeneration;
    qDebug() << "Images { Generation " << mCurrentGeneration << "created! }";
#endif
}

ImageHolder::~ImageHolder() {
#ifdef QT_DEBUG
    qDebug() << "Images { Generation " << mCurrentGeneration << "killed! }";
#endif
    if (!mIsTemporary) {
        return;
    }
    QFile(mFirstImagePath).remove();
    mFirstImage = {};
    if (isPairOfImages()) {
        QFile(mSecondImagePath).remove();
        mSecondImage = {};
    }
}

void ImageHolder::markTemporary() {
    mIsTemporary = true;
}

bool ImageHolder::isMarkedTemporary() const {
    return mIsTemporary;
}

bool ImageHolder::isSingleImage() const {
    return mSecondImage.isNull();
}

bool ImageHolder::isPairOfImages() const {
    return !mFirstImage.isNull() && !mSecondImage.isNull();
}

QPixmap ImageHolder::getFirstImage() const {
    return mFirstImage;
}

QString ImageHolder::getFirstImagePath() const {
    return mFirstImagePath;
}

QPixmap ImageHolder::getSecondImage() const {
    if (isSingleImage()) {
        throw std::runtime_error("ImageHolder contains the single image");
    }
    return mSecondImage;
}

QString ImageHolder::getSecondImagePath() const {
    if (isSingleImage()) {
        throw std::runtime_error("ImageHolder contains the single image");
    }
    return mSecondImagePath;
}
