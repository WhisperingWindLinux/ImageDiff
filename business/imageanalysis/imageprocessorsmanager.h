#ifndef IMAGEPROCESSORSMANAGER_H
#define IMAGEPROCESSORSMANAGER_H

#include <QSet>
#include <business/plugins/pluginsmanager.h>
#include <domain/interfaces/comparator.h>
#include <domain/interfaces/imageprocessor.h>
#include <domain/valueobjects/imageprocessorsinfo.h>

class ImageProcessorsManager
{
public:
    static ImageProcessorsManager *instance();
    void addProcessor(shared_ptr<IImageProcessor> processor);
    shared_ptr<IImageProcessor> findProcessor(QString name);
    QList<ImageProcessorInfo> getAllProcessorsInfo();
    QList<shared_ptr<IComparator> > getAllComparators();
    void clear();

private:
    QList<shared_ptr<IImageProcessor> > processors;
    QSet<QString> hotkeys;
    unique_ptr<PluginsManager> pluginsManager;

    static ImageProcessorsManager *manager;
    ImageProcessorsManager();
    void removeProcessor(QString name);
};

#endif // IMAGEPROCESSORSMANAGER_H
