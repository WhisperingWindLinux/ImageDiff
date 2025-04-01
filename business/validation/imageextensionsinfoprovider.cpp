#include "imageextensionsinfoprovider.h"

#include <qfileinfo.h>

ImageExtensionsInfoProvider::ImageExtensionsInfoProvider()
{
    extensionsForOpen = { "png", "tga" };
    extensionForSave = "png";
}

QString ImageExtensionsInfoProvider::getDeafaultSaveExtension(bool includeDot) {
    if (includeDot) {
        return "." + extensionForSave;
    } else {
        return extensionForSave;
    }
}

QString ImageExtensionsInfoProvider::createOpenFilter() {
    QStringList formattedExtensions;
    foreach (const QString &ext, extensionsForOpen) {
        formattedExtensions.append("*." + ext);
    }
    QString extensionsString = formattedExtensions.join(" ");
    //return QString("%1 Files (%2);;All Files (*.*)").arg("Images", extensionsString);
    return QString("%1 Files (%2)").arg("Images", extensionsString);
}

QString ImageExtensionsInfoProvider::createSaveFilter() {
    return QString("Images Files (*.png)");
}
