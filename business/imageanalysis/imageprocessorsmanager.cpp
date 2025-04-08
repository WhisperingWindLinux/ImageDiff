#include "imageprocessorsmanager.h"

#include <qhash.h>
#include <qsettings.h>

#include <business/imageanalysis/comporators/colorssaturationcomporator.h>
#include <business/imageanalysis/comporators/contrastcomporator.h>
#include <business/imageanalysis/comporators/monocoloreddifferenceinpixelvaluescomporator.h>
#include <business/imageanalysis/comporators/imageproximitytoorigincomparator.h>
#include <business/imageanalysis/comporators/pixelsbrightnesscomparator.h>
#include <business/imageanalysis/comporators/sharpnesscomparator.h>
#include <business/imageanalysis/filters/grayscalefilter.h>
#include <business/imageanalysis/filters/rgbfilter.h>

using namespace std;

ImageProcessorsManager *ImageProcessorsManager::manager = new ImageProcessorsManager();

ImageProcessorsManager *ImageProcessorsManager::instance() {
    return ImageProcessorsManager::manager;
}

ImageProcessorsManager::ImageProcessorsManager() {
    mStorage = new QSettings("com.WhisperingWind", "TwinPix");
}

void ImageProcessorsManager::clear() {
    mProcessors.clear();
    mHotkeys.clear();
}

void ImageProcessorsManager::addProcessor(shared_ptr<IImageProcessor> processor) {
    if (processor == nullptr) {
        return;
    }
    if (mHotkeys.contains(processor->getHotkey()) && !processor->getHotkey().isEmpty()) {
        QString errorMsg = QString("It is not possible to add ") +
                                   "two IImageProcessors with the same hotkey value.";
        throw runtime_error(errorMsg.toStdString());
    }
    mHotkeys.insert(processor->getHotkey());
    mProcessors.append(processor);

    if (processor->getType() != ImageProcessorType::Comparator) {
        return;
    }
    auto comparator = std::static_pointer_cast<IComparator>(processor);
    if (!comparator->isPartOfAutoReportingToolbox()) {
        return;
    }
    QString shortName = processor->getShortName();
    bool isEnabledInAutoanalysisToolbox = mStorage->value(shortName, true).toBool();
    std::static_pointer_cast<IComparator>(processor)->setEnabled(isEnabledInAutoanalysisToolbox);
}

void ImageProcessorsManager::setEnabledInAutoanalysisToolbox(const QString &shortName,
                                                             bool isEnabled
                                                             )
{
    auto processor = this->findProcessorByShortName(shortName);

    if (processor == nullptr || processor->getType() != ImageProcessorType::Comparator) {
        return;
    }
    auto comparator = std::static_pointer_cast<IComparator>(processor);
    if (!comparator->isPartOfAutoReportingToolbox()) {
        return;
    }
    comparator->setEnabled(isEnabled);
    mStorage->setValue(shortName, isEnabled);
}

void ImageProcessorsManager::removeProcessor(QString name) {
    for (auto it = mProcessors.begin(); it != mProcessors.end(); ++it) {
        if ((*it)->getShortName() == name) {
            if (mHotkeys.contains((*it)->getHotkey())) {
                mHotkeys.remove((*it)->getHotkey());
            }
            mProcessors.erase(it);
            break;
        }
    }
}

shared_ptr<IImageProcessor> ImageProcessorsManager::findProcessorByShortName(const QString &name) {
    for (auto it = mProcessors.begin(); it != mProcessors.end(); ++it) {
        if ((*it)->getShortName() == name) {
            return *it;
        }
    }
    return nullptr;
}

shared_ptr<IImageProcessor> ImageProcessorsManager::findProcessorByHotkey(const QChar &hotkey) {
    for (auto it = mProcessors.begin(); it != mProcessors.end(); ++it) {
        if ((*it)->getHotkey() == hotkey) {
            return *it;
        }
    }
    return nullptr;
}

std::optional<ImageProcessorInfo> ImageProcessorsManager::getProcessorInfoByProcessorShortName(const QString &name) {
    auto processor = findProcessorByShortName(name);
    if (processor == nullptr) {
        return std::nullopt;
    }
    ImageProcessorInfo processorInfo(processor->getShortName(),
                                     processor->getFullName(),
                                     processor->getDescription(),
                                     processor->getHotkey(),
                                     processor->getType(),
                                     processor->getDefaultProperties()
                                     );
    return processorInfo;
}

QList<ImageProcessorInfo> ImageProcessorsManager::getAllProcessorsInfo() {
    QList<ImageProcessorInfo> processorsInfo;
    for (auto it = mProcessors.begin(); it != mProcessors.end(); ++it) {
        ImageProcessorInfo processorInfo((*it)->getShortName(),
                                         (*it)->getFullName(),
                                         (*it)->getDescription(),
                                         (*it)->getHotkey(),
                                         (*it)->getType(),
                                         (*it)->getDefaultProperties()
                                         );
        processorsInfo.append(processorInfo);
    }
    return processorsInfo;
}

QList<shared_ptr<IComparator> > ImageProcessorsManager::getAllComparators() {
    QList<shared_ptr<IComparator>> comparators;
    for (auto it = mProcessors.begin(); it != mProcessors.end(); ++it) {
        if ((*it)->getType() == ImageProcessorType::Comparator) {
            comparators.append(dynamic_pointer_cast<IComparator>(*it));
        }
    }
    return comparators;
}
