#ifndef IMAGEPROCESSORSMANAGER_H
#define IMAGEPROCESSORSMANAGER_H

#include <QSet>
#include <business/plugins/pluginsmanager.h>
#include <domain/interfaces/business/icomparator.h>
#include <domain/interfaces/business/imageprocessor.h>
#include <domain/valueobjects/imageprocessorsinfo.h>

class QSettings;

class ImageProcessorsManager
{
public:
    static ImageProcessorsManager *instance();
    void addProcessor(shared_ptr<IImageProcessor> processor);
    shared_ptr<IImageProcessor> findProcessorByShortName(const QString &name);
    shared_ptr<IImageProcessor> findProcessorByHotkey(const QChar &hotkey);
    void setEnabledInAutoanalysisToolbox(const QString &shortName, bool isEnabled);
    std::optional<ImageProcessorInfo> getProcessorInfoByProcessorShortName(const QString &name);
    QList<ImageProcessorInfo> getAllProcessorsInfo();
    QList<shared_ptr<IComparator> > getAllComparators();

    void clear();

private:
    QList<shared_ptr<IImageProcessor> > processors;
    QSet<QString> hotkeys;
    unique_ptr<PluginsManager> pluginsManager;
    QSettings *storage;

    static ImageProcessorsManager *manager;
    ImageProcessorsManager();
    void removeProcessor(QString name);
};

#endif // IMAGEPROCESSORSMANAGER_H
