#include "imageextentionvalidator.h"

#include <qfileinfo.h>

ImageExtentionValidator::ImageExtentionValidator()
{
    extsForOpen = { "png" };
    extForSave = "png";
}

bool ImageExtentionValidator::canOpen(const QString &imagePath) {
    QFileInfo imageInfo {imagePath};
    QString suffix = imageInfo.suffix();
    if (extsForOpen.contains(suffix)) {
        return true;
    } else {
        return false;
    }
}

QString ImageExtentionValidator::getAllExtentionsForUserMessages(bool isToUpper) {
    QString exts;
    bool isFirst = true;
    for (int i = 0; i < extsForOpen.count(); i++) {
        QString ext = extsForOpen[i];
        if (isFirst) {
            isFirst = false;
        } else {
            if (i == extsForOpen.count() - 1) {
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

QString ImageExtentionValidator::getDeafaultSaveExtention(bool includeDot) {
    if (includeDot) {
        return "." + extForSave;
    } else {
        return extForSave;
    }
}

QString ImageExtentionValidator::createFilter() {
    QStringList formattedExtensions;
    foreach (const QString &ext, extsForOpen) {
        formattedExtensions.append("*." + ext);
    }
    QString extensionsString = formattedExtensions.join(" ");
    return QString("%1 Files (%2)").arg("Images", extensionsString);
}
