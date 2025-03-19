#ifndef MAINWINDOWCALLBACKS_H
#define MAINWINDOWCALLBACKS_H

#include <qpixmap.h>
#include <domain/interfaces/iprogressdialog.h>
#include <domain/valueobjects/imageprocessorsinfo.h>
#include <domain/valueobjects/property.h>
#include <domain/valueobjects/imagepixelcolor.h>

class IMainWindowCallbacks : public IProgressDialog {
public:
    virtual void displayImages(QPixmap &image1,
                                             QString path1,
                                             QPixmap &image2,
                                             QString path2) = 0;

    virtual void onImageResultFromComparatorReceived(QPixmap &image, QString description) = 0;

    virtual void onTextResultFromComparatorReceived(QString &message,
                                                    QString comparatorFullName,
                                                    QString comparatorDescription,
                                                    QString &firstImageFilePath,
                                                    QString &secondImageFilePath) = 0;

    virtual void showHelp(QString &message) = 0;

    virtual void onColorUnderCursorChanged(ImagePixelColor firstImageRgbValue,
                                              ImagePixelColor secondImageRgbValue) = 0;

    virtual void onDisplayedImagesShouldBeReplaced(QPixmap& first, QPixmap &second) = 0;

    virtual QList<Property> showImageProcessorPropertiesDialog(QString processorName,
                                                         QString processorDescription,
                                                         QList<Property> defaultProperties) = 0;

    virtual void updateRecentFilesMenu() = 0;

    virtual void saveImage(QPixmap &image, QString defaultPath) = 0;
};


#endif // MAINWINDOWCALLBACKS_H
