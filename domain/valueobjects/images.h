#ifndef IMAGES_H
#define IMAGES_H

#include <qpixmap.h>

struct ImageHolder {

    ImageHolder(const QPixmap &image, const QString &path);

    ImageHolder(const QPixmap &firstImage,
                const QString &firstImagePath,
                const QPixmap &secondImage,
                const QString &secondImagePath);

    ~ImageHolder();

    // When the user selects an area of the image with the mouse while holding
    // down Command, a new instance of the application opens with cropped images.
    // These images are saved in the Temp folder and will be deleted upon closing
    // the application unless the user saves them.
    void markTemporary();
    bool isMarkedTemporary() const;

    bool isSingleImage() const;
    bool isPairOfImages() const;

    QPixmap getFirstImage() const;
    QString getFirstImagePath() const;

    QPixmap getSecondImage() const;
    QString getSecondImagePath() const;

private:
    bool mIsTemporary;

    QPixmap mFirstImage;
    QString mFirstImagePath;

    QPixmap mSecondImage;
    QString mSecondImagePath;

#ifdef QT_DEBUG
    static int mGeneration;
    int mCurrentGeneration;
#endif
};

typedef std::shared_ptr<ImageHolder> ImageHolderPtr;

#endif // IMAGES_H
