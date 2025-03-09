#include <ImageComparator.h>
#include <qfileinfo.h>
#include <stdexcept>

#include "imageviewinteractor.h"


ImageViewInteractor::ImageViewInteractor(QString &firstImgPath, QString &secondImagePath) :
    firstImagePath(firstImgPath),
    secondImagePath(secondImagePath) {
}

QString ImageViewInteractor::getFirstImagePath() const {
    return firstImagePath;
}

QString ImageViewInteractor::getSecondImagePath() const {
    return secondImagePath;
}

QString ImageViewInteractor::getComparisonImagePath() const {
    return comparisonImagePath;
}

QPixmap ImageViewInteractor::getFirstImagePixmap() const {
    return getImagePixmap(0);
}

QPixmap ImageViewInteractor::getSecondImagePixmap() const {
    return getImagePixmap(1);
}

QPixmap ImageViewInteractor::getComparisonImagePixmap() const {
    return getImagePixmap(2);
}

QPixmap ImageViewInteractor::getImagePixmap(int index) const {
    if (images.size() != 3) {
        throw std::runtime_error("Error: Violation of the consistency of the internal structure of the application.");
    }
    return images[index];
}

void ImageViewInteractor::loadImages() {

    images.clear();

    if (!validateFilePath(firstImagePath) || !validateFilePath(secondImagePath)) {
        throw std::runtime_error("Error: Both images must be selected!");
    }

    images.append(QPixmap());
    images.append(QPixmap());

    bool isLoaded1 = images[0].load(firstImagePath);
    bool isLoaded2 = images[1].load(secondImagePath);
    bool isLoaded3 = false;

    if (images[0].size() != images[1].size()) {
        throw std::runtime_error("Error: Images must have the same resolution!");
    }

    ImageComparator comparator(firstImagePath, secondImagePath);
    comparisonImagePath = comparator.compareImagesAndSaveResultAsImage();

    if (validateFilePath(comparisonImagePath)) {
        images.append(QPixmap());
        isLoaded3 = images[2].load(comparisonImagePath);
    } else {
        throw std::runtime_error("Error: Unable to generate the comparison image!");
    }

    if (!isLoaded1 || !isLoaded2 || !isLoaded3) {
        throw std::runtime_error("Error: Unable to load images!");
    }
}

bool ImageViewInteractor::validateFilePath(const QString &filePath) {

    if (filePath.isEmpty()) {
        return false;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return false;
    }

    return true;
}
