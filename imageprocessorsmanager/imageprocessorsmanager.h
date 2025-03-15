#ifndef IMAGEPROCESSORSMANAGER_H
#define IMAGEPROCESSORSMANAGER_H

#include "imageprocessorsinfo.h"
#include <interfaces/comporator.h>
#include <QSet>

class ImageProcessorsManager
{
public:
    static ImageProcessorsManager *instance();
    shared_ptr<IImageProcessor> findProcessor(QString name);
    QList<ImageProcessorInfo> allProcessorsInfo();

    friend class ImageProcessorsManagerTests;

private:
    QList<shared_ptr<IImageProcessor>> processors;
    QSet<QString> hotkeys;

    static ImageProcessorsManager *manager;
    ImageProcessorsManager();
    void addProcessor(shared_ptr<IImageProcessor> comporator);
    void removeProcessor(QString name);
    void addProcessors(QList<shared_ptr<IImageProcessor>> processors);
};

#endif // IMAGEPROCESSORSMANAGER_H
