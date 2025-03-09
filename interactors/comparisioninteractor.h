#ifndef COMPARISIONINTERACTOR_H
#define COMPARISIONINTERACTOR_H

#include <QtCore/qvariant.h>

#include <qpixmap.h>

#include <interfaces/mainwindowcallbacks.h>
class ComparisionInteractor
{
public:
    ComparisionInteractor(AMainWindowCallbacks *callbacks);

    void loadImagesBeingCompared(QString &Image1Path, QString &Image2Path);

    void onImageComporatorShouldBeCalled(QVariant callerData);

private:
    AMainWindowCallbacks *callbacks;
    QString firstImagePath;
    QString secondImagePath;
    QPixmap firstImage;
    QPixmap secondImage;
    QPixmap comparisionImage;

    void clear();
    bool validateFilePath(const QString &filePath);
};

#endif // COMPARISIONINTERACTOR_H
