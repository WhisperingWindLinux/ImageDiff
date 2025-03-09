#include "comparisionmanager.h"

#include "colorssaturationcomporator.h"
#include "contrastcomporator.h"
#include "imagecomparator.h"
#include "pixelsabsolutevaluecomparator.h"
#include "pixelsbrightnesscomparator.h"

ComparisionManager *ComparisionManager::manager = new ComparisionManager();

ComparisionManager *ComparisionManager::instance() {
    return ComparisionManager::manager;
}

ComparisionManager::ComparisionManager() {
    auto imageComparator = new ImageComparator();
    auto imageSaturationComporator = new ColorsSaturationComporator();
    auto imageContrastComporator = new ContrastComporator();
    auto imagePixelsAbsoluteValueComparator = new PixelsAbsoluteValueComparator();
    auto imagePixelsBrightnessComparator = new PixelsBrightnessComparator();

    addComparator(imageComparator);
    addComparator(imageSaturationComporator);
    addComparator(imageContrastComporator);
    addComparator(imagePixelsAbsoluteValueComparator);
    addComparator(imagePixelsBrightnessComparator);
}

void ComparisionManager::addComparator(AComparator *comporator) {
    if (comporator != nullptr) {
        comporators.append(comporator);
    }
}

void ComparisionManager::removeComporator(QString name) {
    for (auto it = comporators.begin(); it != comporators.end(); ++it) {
        if ((*it)->name() == name) {
            comporators.erase(it);
            break;
        }
    }
}

AComparator *ComparisionManager::findComporator(QString name) {
    for (auto it = comporators.begin(); it != comporators.end(); ++it) {
        if ((*it)->name() == name) {
            return *it;
        }
    }
    return nullptr;
}

QList<ComporatorInfo> ComparisionManager::allComporatorsInfo() {
    QList<ComporatorInfo> comporatorsInfo;
    for (auto it = comporators.begin(); it != comporators.end(); ++it) {
        ComporatorInfo comporatorInfo((*it)->name(), (*it)->description(), (*it)->hotkey(), (*it)->contentType());
        comporatorsInfo.append(comporatorInfo);
    }
    return comporatorsInfo;
}
