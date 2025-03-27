#ifndef IMAGEPROCESSINGINTERACTORLISTENER_H
#define IMAGEPROCESSINGINTERACTORLISTENER_H

#include <qpixmap.h>

class IImageProcessingInteractorListener {
public:
    virtual void onComparisonResultLoaded(const QPixmap &image, const QString &description) = 0;

    virtual void onComparisonResultLoaded(const QString &html,
                                          const QString &comparatorFullName,
                                          const QString &firstImagePath,
                                          const QString &secondImagePath) = 0;

    virtual void onFilteredResultLoaded(const QPixmap &firstImage, const QPixmap &secondImage) = 0;
    virtual void onShowImageInExternalViewer(const QPixmap &image, const QString &description) = 0;
    virtual void onImageProcessorFailed(const QString &error) = 0;

    /*
     * The user can switch between images by pressing keys 1, 2, and 3,
     * where the number 3 means switching to the last available
     * comparison result image. However, there are cases when this
     * is not possible: the user has changed the original compared
     * images (applied or removed filters). In this case, the previous
     * comparison result is no longer valid, so the key 3 does not work.
     *
     */
    virtual void onFastSwitchingToComparisonImageStatusChanged(bool isSwitchingAvailable) = 0;
};

#endif // IMAGEPROCESSINGINTERACTORLISTENER_H
