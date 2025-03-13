#ifndef COMPARISONINTERACTOR_H
#define COMPARISONINTERACTOR_H

#include "recentfilesmanager.h"
#include "savefileinfo.h"
#include <interfaces/mainwindowcallbacks.h>

#include <QtCore/qvariant.h>
#include <interfaces/filter.h>
#include <interfaces/comporator.h>
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
    QStringList getRecentFiles();

    friend class ComparisonInteractorTests;

    void loadImagesBeingCompared(QString recentFileMenuRecord);
private:
    AMainWindowCallbacks *callbacks;
    RecentFilesManager *recentFilesManager;
    QString firstImagePath;
    QString secondImagePath;
    QPixmap firstPixmap;
    QPixmap secondPixmap;
    QPixmap comparisionImage;

    void clear();
    bool validateFile(const QString &filePath);
    void callComparator(AComparator *comparator);
    void callFilter(AFilter *transformer);
    void handleProcessorPropertiesIfNeed(AImageProcessor *processor);
};

#endif // COMPARISONINTERACTOR_H
