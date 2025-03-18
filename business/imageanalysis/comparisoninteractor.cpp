#include "comparisoninteractor.h"

#include "imageprocessorsmanager.h"
#include "runallcomparatorsinteractor.h"
#include "setprocessorpropertiesinteractor.h"

#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <business/imageanalysis/comporators/colorssaturationcomporator.h>
#include <business/imageanalysis/comporators/contrastcomporator.h>
#include <business/imageanalysis/comporators/differenceinpixelvaluesasimage.h>
#include <business/imageanalysis/comporators/imageproximitytoorigincomparator.h>
#include <business/imageanalysis/comporators/pixelsabsolutevaluecomparator.h>
#include <business/imageanalysis/comporators/pixelsbrightnesscomparator.h>
#include <business/imageanalysis/comporators/sharpnesscomparator.h>
#include <business/imageanalysis/filters/grayscalefilter.h>
#include <business/imageanalysis/filters/rgbfilter.h>
#include <data/storage/getfileuserpathsservcie.h>
#include <presentation/presenters/htmlimageprocessorshelppresenter.h>
#include <presentation/presenters/recentfilespresenter.h>
#include <qcoreapplication.h>
#include <qfileinfo.h>
#include <tests/testutils.h>

ComparisonInteractor::ComparisonInteractor(IMainWindowCallbacks *callbacks)
    : callbacks(callbacks)
{
    recentFilesManager = new RecentFilesManager("com.whisperingwind", "ImageDiff");
    pluginsManager = new PluginsManager();
    processorsManager = ImageProcessorsManager::instance();
    getImageProcessorsInfo();
}

ComparisonInteractor::~ComparisonInteractor() {
    if (cleanUpImageFilesAtExit) {
        FileUtils::deleteFile(firstImagePath);
        FileUtils::deleteFile(secondImagePath);
    }
}

void ComparisonInteractor::onImageProcessorShouldBeCalled(QVariant callerData) {
    if (!callerData.isValid() || callerData.isNull()) {
        throw std::runtime_error("Error: An incorrect caller data.");
    }

    QString processorName = callerData.toString();

    auto processor = ImageProcessorsManager::instance()->findProcessor(processorName);


    if (processor == nullptr) {
        throw std::runtime_error("Error: Unable to find the requested image processor.");
    }

    processor->reset();

    handleProcessorPropertiesIfNeed(processor);

    if (processor->getType() == ImageProcessorType::Comparator) {
        callComparator(dynamic_pointer_cast<IComparator>(processor));
    } else if (processor->getType() == ImageProcessorType::Filter) {
        callFilter(dynamic_pointer_cast<IFilter>(processor));
    } else {
        throw std::runtime_error("Error: An unknown image processor type.");
    }
}

void ComparisonInteractor::onImageProcessorsHelpShouldBeCalled() {
    auto processorsInfo = processorsManager->getAllProcessorsInfo();
    if (processorsInfo.size() == 0) {
        return;
    }
    auto helpText = HtmlImageProcessorsHelpPresenter::formatToHTML(processorsInfo);
    callbacks->userShouldSeeHelpMessage(helpText);
}

void ComparisonInteractor::handleProcessorPropertiesIfNeed(shared_ptr<IImageProcessor> processor) {
    SetProcessorPropertiesInteractor setPropsInteractor(processor, callbacks);
    setPropsInteractor.allowUserToSetPropertiesIfNeed();
}

void ComparisonInteractor::callComparator(shared_ptr<IComparator> comparator) {

    ComparableImage comapableImage1{firstPixmap, firstImagePath};
    ComparableImage comapableImage2{secondPixmap, secondImagePath};

    auto result = comparator->compare(comapableImage1, comapableImage2);

    if (result.get() == nullptr) {
        throw std::runtime_error("Error: The comparator returns nothing.");
    }

    if (result->type() == ComparisonResultVariantType::Image) {
        QImage imageResult = result->imageResult();
        QPixmap pixmap = QPixmap::fromImage(imageResult);
        if (pixmap.isNull()) {
            throw std::runtime_error("Error: The comparator returns an empty result.");
        }
        callbacks->onImageResultFromComparatorReceived(pixmap, comparator->getShortName());
    }
    else if (result->type() == ComparisonResultVariantType::String) {
        QString stringResult = result->stringResult();
        if (stringResult.isNull() || stringResult.isEmpty()) {
            throw std::runtime_error("Error: The comparator returns an empty result.");
        }
        callbacks->onTextResultFromComparatorReceived(stringResult,
                                                      comparator->getFullName(),
                                                      comparator->getDescription(),
                                                      firstImagePath,
                                                      secondImagePath
                                                      );
    }
}

void ComparisonInteractor::callFilter(shared_ptr<IFilter> filter) {

    QImage image1 = firstPixmap.toImage();
    QImage image2 = secondPixmap.toImage();

    if (image1.isNull() || image2.isNull()) {
        throw std::runtime_error("Error: An error occurred during the loading of one of the images");
    }

    QImage transformedImage1 = filter->filter(image1);
    QImage transformedImage2 = filter->filter(image2);

    if (transformedImage1.isNull() || transformedImage2.isNull()) {
        throw std::runtime_error("Error: The filter returns an empty result.");
    }

    auto pixmap1 = QPixmap::fromImage(transformedImage1);
    auto pixmap2 = QPixmap::fromImage(transformedImage2);

    if (pixmap1.isNull() || pixmap2.isNull()) {
        throw std::runtime_error("Error: The filter returns an empty result.");
    }

    firstPixmap = pixmap1;
    secondPixmap = pixmap2;

    callbacks->onTwoImagesBeingComparedLoadedSuccessfully(firstPixmap,
                                                          firstImagePath,
                                                          secondPixmap,
                                                          secondImagePath,
                                                          true
                                                          );
}

void ComparisonInteractor::onImagesClosed() {
    firstImagePath = {};
    secondImagePath = {};
    firstPixmap = {};
    secondPixmap = {};
    comparisionImage = {};
}

void ComparisonInteractor::reloadImagesFromDisk() {
    loadTwoImagesBeingCompared(firstImagePath, secondImagePath, true, false, false);
}

QStringList ComparisonInteractor::getRecentFiles() {

    QStringList result;
    auto formatter = make_unique<RecentFilesPresenter>();
    auto pairs = recentFilesManager->getAllPairs();


    foreach (auto pair, pairs) {
        auto record = formatter->pairToString(pair);
        if (record.isEmpty()) {
            continue;
        }
        result.append(record);
    }

    return result;
}

void ComparisonInteractor::userRequestOpenTwoImagesBeingCompared() {
    GetFileUserPathsService service;
    OptionalPathPair twoUserImagePaths = service.getUserOpenTwoImagePaths("");
    if (!twoUserImagePaths) {
        return;
    }
    loadTwoImagesBeingCompared(twoUserImagePaths.value().first,
                               twoUserImagePaths.value().second,
                               false,
                               false,
                               true
                               );
}

// Open images from the recent files menu.
// The menu item is formatted as "path to file 1 -> path to file 2".
void ComparisonInteractor::loadTwoImagesBeingCompared(QString recentFileMenuRecord, bool isUpdateRecentMenu) {
    auto formatter = make_unique<RecentFilesPresenter>();
    std::optional<QPair<QString, QString>> pair = formatter->stringToPair(recentFileMenuRecord);
    if (!pair) {
        throw std::runtime_error("Error: Unable to load images!");
    }
    loadTwoImagesBeingCompared(pair->first, pair->second, false, false, isUpdateRecentMenu);
}

void ComparisonInteractor::loadTwoImagesBeingCompared(QList<QUrl> urls) {
    if (urls.size() != 2) {
        return;
    }
    if (urls[0].isLocalFile() && urls[1].isLocalFile()) {
        QString file1 = urls[0].toLocalFile();
        QString file2 = urls[1].toLocalFile();
        loadTwoImagesBeingCompared(file1, file2, false, false, true);
    }
}

void ComparisonInteractor::loadTwoImagesBeingCompared(QString& Image1Path,
                                                      QString& Image2Path,
                                                      bool useSavedImageViewState,
                                                      bool removeImageFilesAtExit,
                                                      bool isUpdateRecentMenu
                                                      )
{

    firstImagePath = Image1Path;
    secondImagePath = Image2Path;
    cleanUpImageFilesAtExit = removeImageFilesAtExit;
    firstPixmap = QPixmap();
    secondPixmap = QPixmap();

    if (!validateFile(firstImagePath) || !validateFile(secondImagePath)) {
        onImagesClosed();
        QString errorMsg = QString("Error: Unable to load images; the files ") +
                            "are missing, or the application does not have access to them!";
        throw std::runtime_error(errorMsg.toStdString());
    }

    bool isLoaded1 = firstPixmap.load(firstImagePath);
    bool isLoaded2 = secondPixmap.load(secondImagePath);

    if (firstPixmap.size() != secondPixmap.size()) {
        onImagesClosed();
        throw std::runtime_error("Error: Images must have the same resolution!");
    }

    if (!isLoaded1 || !isLoaded2) {
        onImagesClosed();
        throw std::runtime_error("Error: Unable to load images!");
    }

    callbacks->onTwoImagesBeingComparedLoadedSuccessfully(firstPixmap,
                                                          firstImagePath,
                                                          secondPixmap,
                                                          secondImagePath,
                                                          useSavedImageViewState
                                                         );

    if (isUpdateRecentMenu) {
        recentFilesManager->addPair(firstImagePath, secondImagePath);
        callbacks->updateRecentFilesMenu();
    }
}

bool ComparisonInteractor::validateFile(const QString &filePath) {

    if (filePath.isEmpty()) {
        return false;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return false;
    }

    static const QStringList supportedFormats = {".png"};
    QString suffix = fileInfo.suffix().toLower();
    if (!supportedFormats.contains("." + suffix)) {
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
    GetFileUserPathsService fsAccess;

    switch (info.saveImageInfoType) {
    case SaveImageInfoType::FirstImage:
        callbacks->saveImage(info.image, firstImagePath);
        break;
    case SaveImageInfoType::SecondImage:
        callbacks->saveImage(info.image, secondImagePath);
        break;
    case SaveImageInfoType::FirstImageArea:
        fileName = file1Name + "_area" + defaultExtention;
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImage(info.image, fullPath);
        break;
    case SaveImageInfoType::SecondImageArea:
        fileName = file2Name + "_area" + defaultExtention;
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImage(info.image, fullPath);
        break;
    case SaveImageInfoType::ComparisonImage:
        fileName = QString("%1_vs_%2_comparison%3")
                       .arg(file1Name, file2Name, defaultExtention);
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImage(info.image, fullPath);
        break;
    case SaveImageInfoType::ComparisonImageArea:
        fileName = QString("%1_vs_%2_area_comparison%3")
                       .arg(file1Name, file2Name, defaultExtention);
        fullPath = defaultDir.filePath(fileName);
        callbacks->saveImage(info.image, fullPath);
        break;
    default:
        break;
    }
}

void ComparisonInteractor::runAllComparators() {

    QFileInfo file1 = QFileInfo(firstImagePath);
    QFileInfo file2 = QFileInfo(secondImagePath);
    QString file1Name = file1.baseName();
    QString file2Name = file2.baseName();
    QDir defaultDir = file1.absoluteDir();
    QString reportDirName = QString("%1_vs_%2_comparison_report")
                                .arg(file1Name)
                                .arg(file2Name);

    QString reportDirPath = defaultDir.absolutePath()
                             + QDir::separator()
                             + reportDirName;

    ComparableImage firstImage {firstPixmap, firstImagePath};
    ComparableImage secondImage {secondPixmap, secondImagePath};
    RunAllComparatorsInteractor runAllComparatorsInteractor {callbacks,
                                                            firstImage,
                                                            secondImage,
                                                            reportDirPath
    };
    runAllComparatorsInteractor.run();
}

QList<ImageProcessorInfo> ComparisonInteractor::getImageProcessorsInfo()
{
    processorsManager->clear();

    // add comporators

    auto imageComparator = make_shared<DifferenceInPixelValuesAsImageComporator>();
    auto imageSaturationComporator = make_shared<ColorsSaturationComporator>();
    auto imageContrastComporator = make_shared<ContrastComporator>();
    auto imagePixelsAbsoluteValueComparator = make_shared<PixelsAbsoluteValueComparator>();
    auto imagePixelsBrightnessComparator = make_shared<PixelsBrightnessComparator>();
    auto sharpnessComparator = make_shared<SharpnessComparator>();
    auto imageProximityComparator = make_shared<ImageProximityToOriginComparator>();

    processorsManager->addProcessor(imageComparator);
    processorsManager->addProcessor(imageSaturationComporator);
    processorsManager->addProcessor(imageContrastComporator);
    processorsManager->addProcessor(imagePixelsAbsoluteValueComparator);
    processorsManager->addProcessor(imagePixelsBrightnessComparator);
    processorsManager->addProcessor(sharpnessComparator);
    processorsManager->addProcessor(imageProximityComparator);

    // add filters

    auto redChannelFilter = make_shared<RedChannelFilter>();
    auto greenChannelFilter = make_shared<GreenChannelFilter>();
    auto blueChannelFilter = make_shared<BlueChannelFilter>();
    auto grayscaleFilter = make_shared<GrayscaleFilter>();

    processorsManager->addProcessor(redChannelFilter);
    processorsManager->addProcessor(greenChannelFilter);
    processorsManager->addProcessor(blueChannelFilter);
    processorsManager->addProcessor(grayscaleFilter);

    auto processors = pluginsManager->loadPlugins();

    foreach (auto processor, processors) {
        processorsManager->addProcessor(processor);
    }
    return processorsManager->getAllProcessorsInfo();
}
