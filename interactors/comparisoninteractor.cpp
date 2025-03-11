#include "comparisoninteractor.h"

#include "savefileinfo.h"
#include "setpropertiesinteractor.h"
#include "../imageprocessorsmanager/imageprocessorsmanager.h"
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <qfileinfo.h>

ComparisonInteractor::ComparisonInteractor(AMainWindowCallbacks *callbacks)
    : callbacks(callbacks)
{

}

void ComparisonInteractor::onImageProcessorShouldBeCalled(QVariant callerData) {
    if (!callerData.isValid() || callerData.isNull()) {
        return;
    }

    QString processorName = callerData.toString();

    auto processor = ImageProcessorsManager::instance()->findProcessor(processorName);
    if (processor == nullptr) {
        return;
    }

    handleProcessorPropertiesIfNeed(processor);

    if (processor->getType() == ImageProcessorType::Comporator) {
        callComparator((AComparator*)processor);
    } else if (processor->getType() == ImageProcessorType::Transformer) {
        callTransformer((ATransformer*)processor);
    } else {
        throw new std::runtime_error("Error: An unknown image processor type.");
    }
}

void ComparisonInteractor::handleProcessorPropertiesIfNeed(AImageProcessor *processor) {
    SetPropertiesInteractor setPropsInteractor(processor, callbacks);
    setPropsInteractor.allowUserToSetPropertiesIfNeed();
}

void ComparisonInteractor::callComparator(AComparator *comparator) {

    ComparableImage comapableImage1{firstPixmap, firstImagePath};
    ComparableImage comapableImage2{secondPixmap, secondImagePath};

    auto result = comparator->compare(comapableImage1, comapableImage2);

    if (result.get() == nullptr) {
        return;
    }

    if (result->type() == ComparisonResultVariantType::Image) {
        QImage imageResult = result->imageResult();
        QPixmap pixmap = QPixmap::fromImage(imageResult);
        if (!pixmap.isNull()) {
            callbacks->onComparisonImagesLoaded(pixmap, comparator->name());
        }
    }
    else if (result->type() == ComparisonResultVariantType::String) {
        QString stringResult = result->stringResult();
        if (stringResult.isNull() || stringResult.isEmpty()) {
            return;
        }
        callbacks->onComparisonTextLoaded(stringResult);
    }
}

void ComparisonInteractor::callTransformer(ATransformer *transformer) {

    QImage image1 = firstPixmap.toImage();
    QImage image2 = secondPixmap.toImage();

    if (image1.isNull() || image2.isNull()) {
        return;
    }

    QImage transformedImage1 = transformer->transform(image1);
    QImage transformedImage2 = transformer->transform(image2);

    if (transformedImage1.isNull() || transformedImage2.isNull()) {
        return;
    }

    auto pixmap1 = QPixmap::fromImage(transformedImage1);
    auto pixmap2 = QPixmap::fromImage(transformedImage2);

    if (pixmap1.isNull() || pixmap2.isNull()) {
        return;
    }

    firstPixmap = pixmap1;
    secondPixmap = pixmap2;

    callbacks->onImagesBeingComparedLoaded(firstPixmap, firstImagePath, secondPixmap, secondImagePath, true);
}

void ComparisonInteractor::clear() {
    firstImagePath = {};
    secondImagePath = {};
    firstPixmap = {};
    secondPixmap = {};
    comparisionImage = {};
}

void ComparisonInteractor::realoadImagesFromDisk() {
    loadImagesBeingCompared(firstImagePath, secondImagePath, true);
}

void ComparisonInteractor::loadImagesBeingCompared(QList<QUrl> urls) {
    if (urls.size() != 2) {
        return;
    }

    QString file1 = urls[0].toLocalFile();
    QString file2 = urls[1].toLocalFile();

    loadImagesBeingCompared(file1, file2, false);
}

void ComparisonInteractor::loadImagesBeingCompared(QString& Image1Path,
                                                   QString& Image2Path,
                                                   bool usePreviousImageGeometry
                                                   )
{

    firstImagePath = Image1Path;
    secondImagePath = Image2Path;
    firstPixmap = QPixmap();
    secondPixmap = QPixmap();

    if (!validateFile(firstImagePath) || !validateFile(secondImagePath)) {
        clear();
        throw std::runtime_error("Error: Both images must be selected!");
    }

    bool isLoaded1 = firstPixmap.load(firstImagePath);
    bool isLoaded2 = secondPixmap.load(secondImagePath);

    if (firstPixmap.size() != firstPixmap.size()) {
        clear();
        throw std::runtime_error("Error: Images must have the same resolution!");
    }

    if (!isLoaded1 || !isLoaded2) {
        clear();
        throw std::runtime_error("Error: Unable to load images!");
    }

    callbacks->onImagesBeingComparedLoaded(firstPixmap,
                                           firstImagePath,
                                           secondPixmap,
                                           secondImagePath,
                                           usePreviousImageGeometry
                                           );
}

bool ComparisonInteractor::validateFile(const QString &filePath) {

    if (filePath.isEmpty()) {
        return false;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return false;
    }

    QString fileName = fileInfo.fileName();
    if(!fileName.contains(".png", Qt::CaseInsensitive)) {
        return false;
    }

    return true;
}

void ComparisonInteractor::saveImage(SaveImageInfo info) {
    if (info.saveImageInfoType == SaveImageInfoType::None || info.image.isNull()) {
        return;
    }

    QFileInfo file1 = QFileInfo(firstImagePath);
    QFileInfo file2 = QFileInfo(secondImagePath);
    QString file1Name = file1.baseName();
    QString file2Name = file2.baseName();
    QDir defaultDir = file1.absoluteDir();
    QString defaultExtention = QString(".") + file1.suffix();
    QString fileName, fullPath;

    switch (info.saveImageInfoType) {
    case SaveImageInfoType::FirstImage:
        callbacks->saveImageAs(info.image, firstImagePath);
        break;
    case SaveImageInfoType::SecondImage:
        callbacks->saveImageAs(info.image, secondImagePath);
        break;
    case SaveImageInfoType::FirstImageArea:
        fileName = file1Name + "_area" + defaultExtention;
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImageAs(info.image, fullPath);
        break;
    case SaveImageInfoType::SecondImageArea:
        fileName = file2Name + "_area" + defaultExtention;
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImageAs(info.image, fullPath);
        break;
    case SaveImageInfoType::ComparisonImage:
        fileName = QString("%1_vs_%2_comparison%3")
                       .arg(file1Name, file2Name, defaultExtention);
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImageAs(info.image, fullPath);
        break;
    case SaveImageInfoType::ComparisonImageArea:
        fileName = QString("%1_vs_%2_areas_comparison%3")
                       .arg(file1Name, file2Name, defaultExtention);
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImageAs(info.image, fullPath);
        break;
    default:
        break;
    }
}
