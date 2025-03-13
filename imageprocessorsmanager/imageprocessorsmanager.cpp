#include "imageprocessorsmanager.h"

#include "comporators/colorssaturationcomporator.h"
#include "comporators/contrastcomporator.h"
#include "comporators/imagecomparator.h"
#include "comporators/pixelsabsolutevaluecomparator.h"
#include "comporators/pixelsbrightnesscomparator.h"
#include <comporators/dummycomparator.h>
#include <comporators/sharpnesscomparator.h>
#include <transformers/grayscaletransformer.h>
#include <transformers/rgbtransformer.h>

ImageProcessorsManager *ImageProcessorsManager::manager = new ImageProcessorsManager();

ImageProcessorsManager *ImageProcessorsManager::instance() {
    return ImageProcessorsManager::manager;
}

ImageProcessorsManager::ImageProcessorsManager() {

    // add comporators

    auto imageComparator = new ImageComparator();
    auto imageSaturationComporator = new ColorsSaturationComporator();
    auto imageContrastComporator = new ContrastComporator();
    auto imagePixelsAbsoluteValueComparator = new PixelsAbsoluteValueComparator();
    auto imagePixelsBrightnessComparator = new PixelsBrightnessComparator();
    auto sharpnessComparator = new SharpnessComparator();

    addProcessor(imageComparator);
    addProcessor(imageSaturationComporator);
    addProcessor(imageContrastComporator);
    addProcessor(imagePixelsAbsoluteValueComparator);
    addProcessor(imagePixelsBrightnessComparator);
    addProcessor(sharpnessComparator);

    #ifdef QT_DEBUG
        auto dummyComparator = new DummyComporator();
        addProcessor(dummyComparator);
    #endif

    // add transformers

    auto redChannelTransformer = new RedChannelTransformer();
    auto greenChannelTransformer = new GreenChannelTransformer();
    auto blueChannelTransformer = new BlueChannelTransformer();
    auto grayscaleTransformer = new GrayscaleTransformer();

    addProcessor(redChannelTransformer);
    addProcessor(greenChannelTransformer);
    addProcessor(blueChannelTransformer);
    addProcessor(grayscaleTransformer);

}

void ImageProcessorsManager::addProcessor(AImageProcessor *comporator) {
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

AImageProcessor *ImageProcessorsManager::findProcessor(QString name) {
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
