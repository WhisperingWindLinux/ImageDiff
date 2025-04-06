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
        saveImageInfoType(saveImageInfoType), image(std::move(image)) {}

    SaveImageInfo() : saveImageInfoType(SaveImageInfoType::None) {
    }

    const SaveImageInfoType saveImageInfoType;
    const QPixmap image;
};

#endif // SAVEFILEINFO_H
