#ifndef IMAGEPROCESSINGINTERACTOR_H
#define IMAGEPROCESSINGINTERACTOR_H

#include <QtCore/qvariant.h>
#include <qpixmap.h>
#include <domain/interfaces/imagefilesinteractorlistener.h>
#include <domain/interfaces/comparator.h>
#include <domain/interfaces/filter.h>
#include <domain/valueobjects/imageprocessorsinfo.h>
#include <domain/valueobjects/savefileinfo.h>
#include <business/recentfilesmanager.h>
#include <domain/interfaces/imageprocessinginteractorlistener.h>


class PluginsManager;
class ImageProcessorsManager;
class IProgressDialog;
class IPropcessorPropertiesDialogCallback;

class ImageProcessingInteractor
{
public:
    ImageProcessingInteractor(const ImagesPtr images,
                              IPropcessorPropertiesDialogCallback *propertiesDialogCallback,
                              IProgressDialog *progressDialogCallback
                              );

    ~ImageProcessingInteractor();

    void callImageProcessor(const QVariant &callerData);
    void runAllComparators();
    void restoreOriginalImages();
    void showImageProcessorsHelp();
    QList<ImageProcessorInfo> getImageProcessorsInfo();

private:
    IPropcessorPropertiesDialogCallback *propertiesDialogCallback;
    IProgressDialog *progressDialogCallback;
    PluginsManager *pluginsManager;
    ImageProcessorsManager *processorsManager;
    QList<IImageProcessorInteractorListenerPtr> listeners;
    ImagesPtr originalImages;
    ImagesPtr displayedImages;

    void coreCallImageProcessor(const QVariant &callerData);
    void callComparator(IComparatorPtr comparator);
    void callFilter(IFilterPtr transformer);
    void handleProcessorPropertiesIfNeed(IImageProcessorPtr processor);

    bool subscribe(const IImageProcessorInteractorListenerPtr listener);
    bool unsubscribe(const IImageProcessorInteractorListenerPtr listener);

    void notifyShowImageProcessorsHelp(const QString &html);
    void notifyComparisonResultLoaded(const QPixmap &image, const QString &description);

    void notifyComparisonResultLoaded(const QString &html,
                                      const QString &comporatorFullName,
                                      const QString &comporatorDescription,
                                      const QString &firstImagePath,
                                      const QString &secondImagePath);

    void notifyFilteredResultLoaded(const QPixmap &firstImage, const QPixmap &secondImage);
    void notifyImageProcessorFailed(const QString &error);
};

#endif // IMAGEPROCESSINGINTERACTOR_H
