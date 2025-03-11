#ifndef COMPARISONINTERACTOR_H
#define COMPARISONINTERACTOR_H

#include "savefileinfo.h"
#include <interfaces/amainwindowcallbacks.h>

#include <QtCore/qvariant.h>
#include <interfaces/ATransformer.h>
#include <interfaces/acomporator.h>
#include <qpixmap.h>

class ComparisonInteractor
{
public:
    ComparisonInteractor(AMainWindowCallbacks *callbacks);

    void loadImagesBeingCompared(QString& Image1Path, QString& Image2Path, bool usePreviousImageGeometry = false);
    void loadImagesBeingCompared(QList<QUrl> urls);
    void onImageProcessorShouldBeCalled(QVariant callerData);
    void saveImage(SaveImageInfo info);
    void realoadImagesFromDisk();

private:
    AMainWindowCallbacks *callbacks;
    QString firstImagePath;
    QString secondImagePath;
    QPixmap firstPixmap;
    QPixmap secondPixmap;
    QPixmap comparisionImage;

    void clear();
    bool validateFile(const QString &filePath);
    void callComparator(AComparator *comparator);
    void callTransformer(ATransformer *transformer);
    void handleProcessorPropertiesIfNeed(AImageProcessor *processor);
};

#endif // COMPARISONINTERACTOR_H
