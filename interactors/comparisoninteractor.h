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
    ComparisonInteractor(IMainWindowCallbacks *callbacks);
    ~ComparisonInteractor();

    void loadTwoImagesBeingCompared(QString& Image1Path,
                                    QString& Image2Path,
                                    bool usePreviousImageGeometry = false,
                                    bool removeFilesAtExit = false
                                    );
    void loadTwoImagesBeingCompared(QList<QUrl> urls);
    void onImageProcessorShouldBeCalled(QVariant callerData);
    void saveImage(SaveImageInfo info);
    void realoadImagesFromDisk();
    QStringList getRecentFiles();

    friend class ComparisonInteractorTests;

    void loadTwoImagesBeingCompared(QString recentFileMenuRecord);
private:
    IMainWindowCallbacks *callbacks;
    RecentFilesManager *recentFilesManager;
    QString firstImagePath;
    QString secondImagePath;
    QPixmap firstPixmap;
    QPixmap secondPixmap;
    QPixmap comparisionImage;
    bool cleanUpImageFilesAtExit = false;

    void clear();
    bool validateFile(const QString &filePath);
    void callComparator(shared_ptr<IComparator> comparator);
    void callFilter(shared_ptr<IFilter> transformer);
    void handleProcessorPropertiesIfNeed(shared_ptr<IImageProcessor>processor);
};

#endif // COMPARISONINTERACTOR_H
