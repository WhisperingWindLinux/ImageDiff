#ifndef SAVEFILEINFO_H
#define SAVEFILEINFO_H

#include <qpixmap.h>

enum class SaveImageInfoType {
    None,
    FirstImage,
    SecondImage,
    FirstImageArea,
    SecondImageArea,
    ComparisonImage,
    ComparisonImageArea
};

struct SaveImageInfo {
    SaveImageInfo(SaveImageInfoType saveImageInfoType, const QPixmap &image) :
        mSaveImageInfoType(saveImageInfoType), mImage(std::move(image)) {}

    SaveImageInfo() : mSaveImageInfoType(SaveImageInfoType::None) {
    }

    const SaveImageInfoType mSaveImageInfoType;
    const QPixmap mImage;
};

typedef std::shared_ptr<QString> QStringPtr;

#endif // SAVEFILEINFO_H
