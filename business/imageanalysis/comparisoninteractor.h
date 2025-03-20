#ifndef COMPARISONINTERACTOR_H
#define COMPARISONINTERACTOR_H

#include <domain/interfaces/imagerepository.h>
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

    void callImageProcessor(const QVariant &callerData, IImagesRepositoryPtr imagesRepository);
    void showImageProcessorsHelp();
    void runAllComparators(IImagesRepositoryPtr imagesRepository);
    QList<ImageProcessorInfo> getImageProcessorsInfo();

private:
    IMainWindowCallbacks *callbacks;
    PluginsManager *pluginsManager;
    ImageProcessorsManager *processorsManager;

    void callComparator(IComparatorPtr comparator, IImagesRepositoryPtr imagesRepository);
    void callFilter(IFilterPtr transformer, IImagesRepositoryPtr imagesRepository);
    void handleProcessorPropertiesIfNeed(IImageProcessorPtr processor);
};

#endif // COMPARISONINTERACTOR_H
