#include "imageprocessorsmanager.h"

#include <qhash.h>

#include <business/imageanalysis/comporators/colorssaturationcomporator.h>
#include <business/imageanalysis/comporators/contrastcomporator.h>
#include <business/imageanalysis/comporators/differenceinpixelvaluesasimage.h>
#include <business/imageanalysis/comporators/imageproximitytoorigincomparator.h>
#include <business/imageanalysis/comporators/pixelsabsolutevaluecomparator.h>
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
}

void ImageProcessorsManager::clear() {
    processors.clear();
    hotkeys.clear();
}

void ImageProcessorsManager::addProcessor(shared_ptr<IImageProcessor> processor) {
    if (processor != nullptr) {
        if (hotkeys.contains(processor->getHotkey())) {
            QString errorMsg = QString("Error: It is not possible to add ") +
                                       "two IImageProcessors with the same hotkey value.";
            throw runtime_error(errorMsg.toStdString());
        }
        hotkeys.insert(processor->getHotkey());
        processors.append(processor);
    }
}

void ImageProcessorsManager::removeProcessor(QString name) {
    for (auto it = processors.begin(); it != processors.end(); ++it) {
        if ((*it)->getShortName() == name) {
            if (hotkeys.contains((*it)->getHotkey())) {
                hotkeys.remove((*it)->getHotkey());
            }
            processors.erase(it);
            break;
        }
    }
}

shared_ptr<IImageProcessor> ImageProcessorsManager::findProcessor(QString name) {
    for (auto it = processors.begin(); it != processors.end(); ++it) {
        if ((*it)->getShortName() == name) {
            return *it;
        }
    }
    return nullptr;
}

QList<ImageProcessorInfo> ImageProcessorsManager::getAllProcessorsInfo() {
    QList<ImageProcessorInfo> processorsInfo;
    for (auto it = processors.begin(); it != processors.end(); ++it) {
        ImageProcessorInfo processorInfo((*it)->getShortName(),
                                         (*it)->getDescription(),
                                         (*it)->getHotkey(),
                                         (*it)->getType()
                                         );
        processorsInfo.append(processorInfo);
    }
    return processorsInfo;
}

QList<shared_ptr<IComparator> > ImageProcessorsManager::getAllComparators() {
    QList<shared_ptr<IComparator>> comparators;
    for (auto it = processors.begin(); it != processors.end(); ++it) {
        if ((*it)->getType() == ImageProcessorType::Comparator) {
            comparators.append(dynamic_pointer_cast<IComparator>(*it));
        }
    }
    return comparators;
}
