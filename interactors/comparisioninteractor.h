#ifndef COMPARISIONINTERACTOR_H
#define COMPARISIONINTERACTOR_H

#include <QtCore/qvariant.h>

#include <qpixmap.h>
#include <savefileinfo.h>

#include <interfaces/mainwindowcallbacks.h>

class ComparisionInteractor
{
public:
    ComparisionInteractor(AMainWindowCallbacks *callbacks);

    void loadImagesBeingCompared(QString &Image1Path, QString &Image2Path);

    void onImageComporatorShouldBeCalled(QVariant callerData);

    void saveImage(SaveImageInfo info);

    void onDisplayedRgbChannelsChanged(RgbChannels channels, ImageGeometry imageGeometry);

private:
    AMainWindowCallbacks *callbacks;
    QString firstImagePath;
    QString secondImagePath;
    QPixmap firstImage;
    QPixmap secondImage;
    QPixmap comparisionImage;

    void clear();
    bool validateFilePath(const QString &filePath);
    QPixmap extractChannel(const QPixmap& pixmap, RgbChannels channels);
};

#endif // COMPARISIONINTERACTOR_H
