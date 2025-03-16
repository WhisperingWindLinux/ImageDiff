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
    SaveImageInfo(SaveImageInfoType saveImageInfoType, QPixmap image) :
        saveImageInfoType(saveImageInfoType), image(image) {}

    SaveImageInfo() {
        saveImageInfoType = SaveImageInfoType::None;
    }

    SaveImageInfoType saveImageInfoType;
    QPixmap image;
};

#endif // SAVEFILEINFO_H
