#include "imageprocessorsmanager.h"

#include "comporators/colorssaturationcomporator.h"
#include "comporators/contrastcomporator.h"
#include "comporators/imagecomparator.h"
#include "comporators/pixelsabsolutevaluecomparator.h"
#include "comporators/pixelsbrightnesscomparator.h"
#include <comporators/dummycomparator.h>
#include <comporators/imageproximitytoorigincomparator.h>
#include <comporators/sharpnesscomparator.h>
#include <filters/grayscalefilter.h>
#include <filters/rgbfilter.h>

ImageProcessorsManager *ImageProcessorsManager::manager = new ImageProcessorsManager();

ImageProcessorsManager *ImageProcessorsManager::instance() {
    return ImageProcessorsManager::manager;
}

ImageProcessorsManager::ImageProcessorsManager() {

    pluginInteractor = make_unique<PluginInteractor>();

    // add comporators

    auto imageComparator = make_shared<ImageComparator>();
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

    #ifdef QT_DEBUG
        auto dummyComparator = make_shared<DummyComporator>();
        addProcessor(dummyComparator);
    #endif

    // add transformers

    auto redChannelFilter = make_shared<RedChannelFilter>();
    auto greenChannelFilter = make_shared<GreenChannelFilter>();
    auto blueChannelFilter = make_shared<BlueChannelFilter>();
    auto grayscaleFilter = make_shared<GrayscaleFilter>();

    addProcessor(redChannelFilter);
    addProcessor(greenChannelFilter);
    addProcessor(blueChannelFilter);
    addProcessor(grayscaleFilter);

    auto processorsFromPlugins = pluginInteractor->getProcessors();
    foreach (auto processor, processorsFromPlugins) {
        addProcessor(processor);
    }
}

void ImageProcessorsManager::addProcessor(shared_ptr<IImageProcessor> comporator) {
    if (comporator != nullptr) {
        processors.append(comporator);
    }
}

void ImageProcessorsManager::removeProcessor(QString name) {
    for (auto it = processors.begin(); it != processors.end(); ++it) {
        if ((*it)->name() == name) {
            processors.erase(it);
            break;
        }
    }
}

shared_ptr<IImageProcessor> ImageProcessorsManager::findProcessor(QString name) {
    for (auto it = processors.begin(); it != processors.end(); ++it) {
        if ((*it)->name() == name) {
            return *it;
        }
    }
    return nullptr;
}

QList<ImageProcessorInfo> ImageProcessorsManager::allProcessorsInfo() {
    QList<ImageProcessorInfo> processorsInfo;
    for (auto it = processors.begin(); it != processors.end(); ++it) {
        ImageProcessorInfo processorInfo((*it)->name(), (*it)->description(), (*it)->hotkey(), (*it)->getType());
        processorsInfo.append(processorInfo);
    }
    return processorsInfo;
}
