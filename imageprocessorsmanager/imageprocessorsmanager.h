#ifndef IMAGEPROCESSORSMANAGER_H
#define IMAGEPROCESSORSMANAGER_H

#include "imageprocessorsinfo.h"
#include <interfaces/comporator.h>

class ImageProcessorsManager
{
public:
    static ImageProcessorsManager *instance();
    AImageProcessor *findProcessor(QString name);
    QList<ImageProcessorInfo> allProcessorsInfo();

    friend class ImageProcessorsManagerTests;

private:
    QList<AImageProcessor*> processors;

    static ImageProcessorsManager *manager;
    ImageProcessorsManager();
    void addProcessor(AImageProcessor *comporator);
    void removeProcessor(QString name);
};

#endif // IMAGEPROCESSORSMANAGER_H
