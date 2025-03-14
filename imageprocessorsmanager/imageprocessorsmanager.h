#ifndef IMAGEPROCESSORSMANAGER_H
#define IMAGEPROCESSORSMANAGER_H

#include "imageprocessorsinfo.h"
#include <interfaces/comporator.h>

class ImageProcessorsManager
{
public:
    static ImageProcessorsManager *instance();
    shared_ptr<IImageProcessor> findProcessorByName(QString name);
    QList<ImageProcessorInfo> allProcessorsInfo();

    friend class ImageProcessorsManagerTests;

    shared_ptr<IImageProcessor> findProcessorByHotkey(QString hotkey);
private:
    QList<shared_ptr<IImageProcessor>> processors;

    static ImageProcessorsManager *manager;
    ImageProcessorsManager();
    void addProcessor(shared_ptr<IImageProcessor> comporator);
    void removeProcessor(QString name);
    void addProcessors(QList<shared_ptr<IImageProcessor>> processors);
};

#endif // IMAGEPROCESSORSMANAGER_H
