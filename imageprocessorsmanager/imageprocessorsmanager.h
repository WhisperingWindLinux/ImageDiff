#ifndef IMAGEPROCESSORSMANAGER_H
#define IMAGEPROCESSORSMANAGER_H

#include "imageprocessorsinfo.h"
#include <interactors/plugininteractor.h>
#include <interfaces/comporator.h>
#include <plugins/pluginmanager.h>

class ImageProcessorsManager
{
public:
    static ImageProcessorsManager *instance();
    shared_ptr<IImageProcessor> findProcessor(QString name);
    QList<ImageProcessorInfo> allProcessorsInfo();

    friend class ImageProcessorsManagerTests;

private:
    QList<shared_ptr<IImageProcessor>> processors;
    unique_ptr<PluginInteractor> pluginInteractor;

    static ImageProcessorsManager *manager;
    ImageProcessorsManager();
    void addProcessor(shared_ptr<IImageProcessor> comporator);
    void removeProcessor(QString name);    
};

#endif // IMAGEPROCESSORSMANAGER_H
