#ifndef MAINWINDOWCALLBACKS_H
#define MAINWINDOWCALLBACKS_H

#include <qpixmap.h>

#include <imageprocessorsmanager/property.h>

#include <gui/RgbValue.h>

class AMainWindowCallbacks {
public:
    virtual void onTwoImagesBeingComparedLoadedSuccessfully(QPixmap &image1,
                                             QString path1,
                                             QPixmap &image2,
                                             QString path2,
                                             bool usePreviousImageGeometry) = 0;

    virtual void onImageResultFromComparatorReceived(QPixmap &image,
                                          QString description) = 0;

    virtual void onTextResultFromComparatorReceived(QString text) = 0;

    virtual void saveImage(QPixmap &image, QString defaultPath) = 0;

    virtual void onRgbValueUnderCursonChanged(RgbValue firstImageRgbValue, RgbValue secondImageRgbValue) = 0;

    virtual QList<Property> getUpdatedPropertiesFromUser(QList<Property> defaultProperties) = 0;

    virtual void updateRecentFilesMenu() = 0;
};


#endif // MAINWINDOWCALLBACKS_H
