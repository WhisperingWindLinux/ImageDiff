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
    friend class ComparisonInteractorTests;

    ComparisonInteractor(IMainWindowCallbacks *callbacks);
    ~ComparisonInteractor();

    void openImages(QString& Image1Path,
                                    QString& Image2Path,
                                    bool removeFilesAtExit,
                                    bool isUpdateRecentMenu
                                    );

    void openImagesFromDragAndDrop(QList<QUrl> urls);
    void openImagesFromRecentMenu(QString recentFileMenuRecord, bool isUpdateRecentMenu);
    void callImageProcessor(QVariant callerData);
    void showImageProcessorsHelp();
    void saveImage(SaveImageInfo info);
    QStringList getRecentFiles();
    void runAllComparators();
    void getPathsFromUserAndOpenImages();
    QList<ImageProcessorInfo> getImageProcessorsInfo();


    void showOriginalImages();
private:
    IMainWindowCallbacks *callbacks;
    RecentFilesManager *recentFilesManager;
    PluginsManager *pluginsManager;
    ImageProcessorsManager *processorsManager;
    QString firstImagePath;
    QString secondImagePath;
    QPixmap originalFirstPixmap;
    QPixmap originalSecondPixmap;
    QPixmap firstCurrentlyDisplayedPixmap;
    QPixmap secondCurrentlyDisplayedPixmap;
    QPixmap comparisionImage;
    bool cleanUpImageFilesAtExit = false;

    bool validateFile(const QString &filePath);
    void callComparator(shared_ptr<IComparator> comparator);
    void callFilter(shared_ptr<IFilter> transformer);
    void handleProcessorPropertiesIfNeed(shared_ptr<IImageProcessor>processor);

    void cleanUp();
};

#endif // COMPARISONINTERACTOR_H
