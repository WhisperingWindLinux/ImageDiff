#ifndef IMAGES_INFO
#define IMAGES_INFO

#include <QtCore/qdir.h>
#include <domain/valueobjects/images.h>
#include <qfileinfo.h>
#include <qstring.h>

class ImagesInfo {
public:
    ImagesInfo(const ImageHolderPtr images);
    ~ImagesInfo() = default;

    QString getFirstImagePath() const;
    QString getSecondImagePath() const;
    QString getFirstImageName() const;
    QString getSecondImageName() const;
    QString getFirstImageBaseName() const;
    QString getSecondImageBaseName() const;
    QString getFirstImageDir() const;
    QString getSecondImageDir() const;

private:
    QString mFirstImagePath;
    QString mSecondImagePath;
    QString mFirstImageName;
    QString mSecondImageName;
    QString mFirstImageBaseName;
    QString mSecondImageBaseName;
    QString mFirstImageDir;
    QString mSecondImageDir;
};

#endif // IMAGES_INFO
