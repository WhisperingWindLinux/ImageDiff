#ifndef COMPARISONINTERACTOR_H
#define COMPARISONINTERACTOR_H

#include <QtCore/qvariant.h>
#include <qpixmap.h>
#include <domain/interfaces/comparator.h>
#include <domain/interfaces/filter.h>
#include <domain/interfaces/mainwindowcallbacks.h>
#include <domain/valueobjects/savefileinfo.h>
#include <business/recentfilesmanager.h>

class PluginsManager;
class ImageProcessorsManager;

class ComparisonInteractor
{
public:
    ComparisonInteractor(IMainWindowCallbacks *callbacks);
    ~ComparisonInteractor();

    void loadTwoImagesBeingCompared(QString& Image1Path,
                                    QString& Image2Path,
                                    bool useSavedImageViewState,
                                    bool removeFilesAtExit,
                                    bool isUpdateRecentMenu
                                    );

    void loadTwoImagesBeingCompared(QList<QUrl> urls);
    void loadTwoImagesBeingCompared(QString recentFileMenuRecord, bool isUpdateRecentMenu);
    void onImageProcessorShouldBeCalled(QVariant callerData);
    void onImageProcessorsHelpShouldBeCalled();
    void saveImage(SaveImageInfo info);
    void reloadImagesFromDisk();
    QStringList getRecentFiles();
    void runAllComparators();
    void userRequestOpenTwoImagesBeingCompared();
    QList<ImageProcessorInfo> getImageProcessorsInfo();

    friend class ComparisonInteractorTests;

private:
    IMainWindowCallbacks *callbacks;
    RecentFilesManager *recentFilesManager;
    PluginsManager *pluginsManager;
    ImageProcessorsManager *processorsManager;
    QString firstImagePath;
    QString secondImagePath;
    QPixmap firstPixmap;
    QPixmap secondPixmap;
    QPixmap comparisionImage;
    bool cleanUpImageFilesAtExit = false;

    void onImagesClosed();
    bool validateFile(const QString &filePath);
    void callComparator(shared_ptr<IComparator> comparator);
    void callFilter(shared_ptr<IFilter> transformer);
    void handleProcessorPropertiesIfNeed(shared_ptr<IImageProcessor>processor);
};

#endif // COMPARISONINTERACTOR_H
