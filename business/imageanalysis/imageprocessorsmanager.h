#ifndef IMAGEPROCESSORSMANAGER_H
#define IMAGEPROCESSORSMANAGER_H

#include <QSet>

#include <domain/interfaces/comporator.h>
#include <domain/interfaces/imageprocessor.h>

#include <domain/valueobjects/imageprocessorsinfo.h>

class ImageProcessorsManager
{
public:
    static ImageProcessorsManager *instance();
    shared_ptr<IImageProcessor> findProcessor(QString name);
    QList<ImageProcessorInfo> getAllProcessorsInfo();
    QList<shared_ptr<IComparator> > getAllComparators();

    friend class ImageProcessorsManagerTests;

private:
    QList<shared_ptr<IImageProcessor> > processors;
    QSet<QString> hotkeys;

    static ImageProcessorsManager *manager;
    ImageProcessorsManager();
    void addProcessor(shared_ptr<IImageProcessor> comporator);
    void removeProcessor(QString name);
    void addProcessors(QList<shared_ptr<IImageProcessor> > processors);
};

#endif // IMAGEPROCESSORSMANAGER_H
