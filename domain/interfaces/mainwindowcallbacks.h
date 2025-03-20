#ifndef MAINWINDOWCALLBACKS_H
#define MAINWINDOWCALLBACKS_H

#include <qpixmap.h>
#include <domain/interfaces/iprogressdialog.h>
#include <domain/valueobjects/imageprocessorsinfo.h>
#include <domain/valueobjects/property.h>
#include <domain/valueobjects/imagepixelcolor.h>

class IMainWindowCallbacks : public IProgressDialog {
public:
    virtual void displayImages(const QPixmap &image1,
                               const QString &path1,
                               const QPixmap &image2,
                               const QString &path2) = 0;

    virtual void onImageResultFromComparatorReceived(const QPixmap &image,
                                                     const QString &description) = 0;

    virtual void onTextResultFromComparatorReceived(const QString &message,
                                                    const QString &comparatorFullName,
                                                    const QString &comparatorDescription,
                                                    const QString &firstImageFilePath,
                                                    const QString &secondImageFilePath) = 0;

    virtual void showHelp(const QString &message) = 0;

    virtual void onColorUnderCursorChanged(ImagePixelColor firstImageRgbValue,
                                           ImagePixelColor secondImageRgbValue) = 0;

    virtual void onDisplayedImagesShouldBeReplaced(const QPixmap &first, const QPixmap &second) = 0;

    virtual QList<Property> showImageProcessorPropertiesDialog(const QString &processorName,
                                                               const QString &processorDescription,
                                                               const QList<Property> &defaultProperties) = 0;

    virtual void updateRecentFilesMenu() = 0;

    virtual void saveImage(const QPixmap &image, const QString &defaultPath) = 0;
};


#endif // MAINWINDOWCALLBACKS_H
