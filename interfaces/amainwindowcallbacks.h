#ifndef AMAINWINDOWCALLBACKS_H
#define AMAINWINDOWCALLBACKS_H

#include <qpixmap.h>

#include <imageprocessorsmanager/property.h>

class AMainWindowCallbacks {
public:    
    virtual void onImagesBeingComparedLoaded(QPixmap &image1,
                                             QString path1,
                                             QPixmap &image2,
                                             QString path2,
                                             bool usePreviousImageGeometry) = 0;

    virtual void onComparisonImagesLoaded(QPixmap &image,
                                          QString description) = 0;

    virtual void onComparisonTextLoaded(QString text) = 0;

    virtual void saveImageAs(QPixmap &image, QString defaultPath) = 0;

    virtual void onRgbValueUnderCursonChanged(QString imageName, int r, int g, int b) = 0;

    virtual QList<Property> getUpdatedPropertiesFromUser(QList<Property> defaultProperties) = 0;
};


#endif // AMAINWINDOWCALLBACKS_H
