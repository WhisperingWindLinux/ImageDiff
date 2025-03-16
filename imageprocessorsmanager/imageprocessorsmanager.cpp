#include "imageprocessorsmanager.h"

#include "comporators/colorssaturationcomporator.h"
#include "comporators/contrastcomporator.h"
#include "comporators/differenceinpixelvaluesasimage.h"
#include "comporators/pixelsabsolutevaluecomparator.h"
#include "comporators/pixelsbrightnesscomparator.h"
#include <comporators/imageproximitytoorigincomparator.h>
#include <comporators/sharpnesscomparator.h>
#include <filters/grayscalefilter.h>
#include <filters/rgbfilter.h>
#include <qhash.h>

using namespace std;

ImageProcessorsManager *ImageProcessorsManager::manager = new ImageProcessorsManager();

ImageProcessorsManager *ImageProcessorsManager::instance() {
    return ImageProcessorsManager::manager;
}

ImageProcessorsManager::ImageProcessorsManager() {

    // add comporators

    auto imageComparator = make_shared<DifferenceInPixelValuesAsImageComporator>();
    auto imageSaturationComporator = make_shared<ColorsSaturationComporator>();
    auto imageContrastComporator = make_shared<ContrastComporator>();
    auto imagePixelsAbsoluteValueComparator = make_shared<PixelsAbsoluteValueComparator>();
    auto imagePixelsBrightnessComparator = make_shared<PixelsBrightnessComparator>();
    auto sharpnessComparator = make_shared<SharpnessComparator>();
    auto imageProximityComparator = make_shared<ImageProximityToOriginComparator>();

    addProcessor(imageComparator);
    addProcessor(imageSaturationComporator);
    addProcessor(imageContrastComporator);
    addProcessor(imagePixelsAbsoluteValueComparator);
    addProcessor(imagePixelsBrightnessComparator);
    addProcessor(sharpnessComparator);
    addProcessor(imageProximityComparator);

    // add filters

    auto redChannelFilter = make_shared<RedChannelFilter>();
    auto greenChannelFilter = make_shared<GreenChannelFilter>();
    auto blueChannelFilter = make_shared<BlueChannelFilter>();
    auto grayscaleFilter = make_shared<GrayscaleFilter>();

    addProcessor(redChannelFilter);
    addProcessor(greenChannelFilter);
    addProcessor(blueChannelFilter);
    addProcessor(grayscaleFilter);
}

void ImageProcessorsManager::addProcessor(shared_ptr<IImageProcessor> comporator) {
    if (comporator != nullptr) {
        if (hotkeys.contains(comporator->getHotkey())) {
            QString errorMsg = QString("Error: It is not possible to add ") +
                                       "two IImageProcessors with the same hotkey value.";
            throw runtime_error(errorMsg.toStdString());
        }
        hotkeys.insert(comporator->getHotkey());
        processors.append(comporator);
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







