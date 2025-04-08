#include "imageextensionsinfoprovider.h"

#include <qfileinfo.h>

ImageExtensionsInfoProvider::ImageExtensionsInfoProvider() {
    mExtensionsForOpen = { "png", "tga", "jpg", "jpeg", "bmp", "gif" };
    mExtensionForSave = "png";
}

QString ImageExtensionsInfoProvider::getDeafaultSaveExtension(bool includeDot) {
    if (includeDot) {
        return "." + mExtensionForSave;
    } else {
        return mExtensionForSave;
    }
}

QString ImageExtensionsInfoProvider::createOpenFilter() {
    QStringList formattedExtensions;
    foreach (const QString &ext, mExtensionsForOpen) {
        formattedExtensions.append("*." + ext);
    }
    QString extensionsString = formattedExtensions.join(" ");
    return QString("%1 Files (%2)").arg("Images", extensionsString);
}

QString ImageExtensionsInfoProvider::createSaveFilter() {
    return QString("Images Files (*.png)");
}
