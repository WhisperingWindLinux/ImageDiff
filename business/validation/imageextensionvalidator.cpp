#include "imageextensionvalidator.h"

#include <qfileinfo.h>

ImageExtensionValidator::ImageExtensionValidator()
{
    extensionsForOpen = { "png" };
    extensionForSave = "png";
}

bool ImageExtensionValidator::canOpen(const QString &imagePath) {
    QFileInfo imageInfo { imagePath };
    QString suffix = imageInfo.suffix();
    if (extensionsForOpen.contains(suffix)) {
        return true;
    } else {
        return false;
    }
}

QString ImageExtensionValidator::getAllExtensionsForUserMessages(bool isToUpper) {
    QString exts;
    bool isFirst = true;
    for (int i = 0; i < extensionsForOpen.count(); i++) {
        QString ext = extensionsForOpen[i];
        if (isFirst) {
            isFirst = false;
        } else {
            if (i == extensionsForOpen.count() - 1) {
                exts.append(" or ");
            } else {
                exts.append(", ");
            }
        }
        if (isToUpper) {
            ext = ext.toUpper();
        }
        exts.append(ext);
    }
    return exts;
}

QString ImageExtensionValidator::getDeafaultSaveExtension(bool includeDot) {
    if (includeDot) {
        return "." + extensionForSave;
    } else {
        return extensionForSave;
    }
}

QString ImageExtensionValidator::createFilter() {
    QStringList formattedExtensions;
    foreach (const QString &ext, extensionsForOpen) {
        formattedExtensions.append("*." + ext);
    }
    QString extensionsString = formattedExtensions.join(" ");
    return QString("%1 Files (%2)").arg("Images", extensionsString);
}
