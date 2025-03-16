#ifndef COMPARISONINTERACTOR_H
#define COMPARISONINTERACTOR_H

#include <QtCore/qvariant.h>
#include <qpixmap.h>
#include <domain/interfaces/comporator.h>
#include <domain/interfaces/filter.h>
#include <domain/interfaces/mainwindowcallbacks.h>
#include <domain/valueobjects/savefileinfo.h>
#include <business/recentfilesmanager.h>

class ComparisonInteractor
{
public:
    ComparisonInteractor(IMainWindowCallbacks *callbacks);
    ~ComparisonInteractor();

    void loadTwoImagesBeingCompared(QString& Image1Path,
                                    QString& Image2Path,
                                    bool usePreviousImageGeometry,
                                    bool removeFilesAtExit,
                                    bool isUpdateRecentMenu
                                    );

    void loadTwoImagesBeingCompared(QList<QUrl> urls);
    void onImageProcessorShouldBeCalled(QVariant callerData);
    void onImageProcessorHelpShouldBeCalled(QVariant callerData);
    void saveImage(SaveImageInfo info);
    void realoadImagesFromDisk();
    QStringList getRecentFiles();
    void runAllComparators();

    friend class ComparisonInteractorTests;

    void loadTwoImagesBeingCompared(QString recentFileMenuRecord, bool isUpdateRecentMenu);
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
