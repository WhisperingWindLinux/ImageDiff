#include "comparisoninteractor.h"

#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <qcoreapplication.h>
#include <qfileinfo.h>
#include <business/imageanalysis/comporators/colorssaturationcomporator.h>
#include <business/imageanalysis/comporators/contrastcomporator.h>
#include <business/imageanalysis/comporators/differenceinpixelvaluesasimage.h>
#include <business/imageanalysis/comporators/imageproximitytoorigincomparator.h>
#include <business/imageanalysis/comporators/pixelsabsolutevaluecomparator.h>
#include <business/imageanalysis/comporators/pixelsbrightnesscomparator.h>
#include <business/imageanalysis/comporators/sharpnesscomparator.h>
#include <business/imageanalysis/filters/grayscalefilter.h>
#include <business/imageanalysis/filters/rgbfilter.h>
#include <data/storage/savefiledialoghandler.h>
#include <presentation/presenters/htmlimageprocessorshelppresenter.h>
#include <domain/interfaces/imagerepository.h>
#include "imageprocessorsmanager.h"
#include "runallcomparatorsinteractor.h"
#include "setprocessorpropertiesinteractor.h"

ComparisonInteractor::ComparisonInteractor(IMainWindowCallbacks *callbacks)
    : callbacks(callbacks)
{
    pluginsManager = new PluginsManager();
    processorsManager = ImageProcessorsManager::instance();
    getImageProcessorsInfo();
}

ComparisonInteractor::~ComparisonInteractor() {
    if (pluginsManager != nullptr) {
        delete pluginsManager;
        pluginsManager = nullptr;
    }
    if (processorsManager != nullptr) {
        delete processorsManager;
        processorsManager = nullptr;
    }
}

void ComparisonInteractor::callImageProcessor(const QVariant &callerData,
                                              IImagesRepositoryPtr imagesRepository
                                              )
{
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
        callComparator(dynamic_pointer_cast<IComparator>(processor), imagesRepository);
    } else if (processor->getType() == ImageProcessorType::Filter) {
        callFilter(dynamic_pointer_cast<IFilter>(processor), imagesRepository);
    } else {
        throw std::runtime_error("Error: An unknown image processor type.");
    }
}

void ComparisonInteractor::showImageProcessorsHelp() {
    auto processorsInfo = processorsManager->getAllProcessorsInfo();
    if (processorsInfo.size() == 0) {
        return;
    }
    auto helpText = HtmlImageProcessorsHelpPresenter::formatToHTML(processorsInfo);
    callbacks->showHelp(helpText);
}

void ComparisonInteractor::handleProcessorPropertiesIfNeed(IImageProcessorPtr processor) {
    SetProcessorPropertiesInteractor setPropsInteractor(processor, callbacks);
    setPropsInteractor.allowUserToSetPropertiesIfNeed();
}

void ComparisonInteractor::callComparator(IComparatorPtr comparator,
                                          IImagesRepositoryPtr imagesRepository
                                          )
{

    auto firstImage = imagesRepository->getFirstImage();
    auto firstImagePath = imagesRepository->getFirstImagePath();
    auto secondImage = imagesRepository->getSecondImage();
    auto secondImagePath = imagesRepository->getSecondImagePath();

    ComparableImage comapableImage1{ firstImage, firstImagePath };
    ComparableImage comapableImage2{ secondImage, secondImagePath };

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

void ComparisonInteractor::callFilter(shared_ptr<IFilter> filter,
                                      IImagesRepositoryPtr imagesRepository
                                      )
{
    auto firstImage = imagesRepository->getFirstImage().toImage();
    auto secondImage = imagesRepository->getSecondImage().toImage();

    if (firstImage.isNull() || secondImage.isNull()) {
        throw std::runtime_error("Error: An error occurred during the loading of one of the images");
    }

    QImage transformedImage1 = filter->filter(firstImage);
    QImage transformedImage2 = filter->filter(secondImage);

    if (transformedImage1.isNull() || transformedImage2.isNull()) {
        throw std::runtime_error("Error: The filter returns an empty result.");
    }

    auto pixmap1 = QPixmap::fromImage(transformedImage1);
    auto pixmap2 = QPixmap::fromImage(transformedImage2);

    if (pixmap1.isNull() || pixmap2.isNull()) {
        throw std::runtime_error("Error: The filter returns an empty result.");
    }

    imagesRepository->update(pixmap1, pixmap2);

    callbacks->onDisplayedImagesShouldBeReplaced(pixmap1, pixmap2);
}

void ComparisonInteractor::runAllComparators(IImagesRepositoryPtr imagesRepository) {

    auto name1 = imagesRepository->getFirstImageBaseName();
    auto name2 = imagesRepository->getSecondImageBaseName();

    QString reportDirName = QString("%1_vs_%2_comparison_report").arg(name1, name2);

    QString saveReportDirPath = imagesRepository->getFirstImageDir()
                                + QDir::separator()
                                + reportDirName;

    auto image1 = imagesRepository->getFirstImage();
    auto image2 = imagesRepository->getSecondImage();
    auto path1 = imagesRepository->getFirstImagePath();
    auto path2 = imagesRepository->getSecondImagePath();

    ComparableImage firstComparableImage {image1, path1};
    ComparableImage secondComparableImage {image2, path2};

    RunAllComparatorsInteractor runAllComparatorsInteractor { callbacks,
                                                              firstComparableImage,
                                                              secondComparableImage,
                                                              saveReportDirPath
                                                            };

    runAllComparatorsInteractor.run();
}

QList<ImageProcessorInfo> ComparisonInteractor::getImageProcessorsInfo() {
    processorsManager->clear();

    // add comparators

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

    // add plugins (comparators and filters)

    auto processors = pluginsManager->loadPlugins();

    foreach (auto processor, processors) {
        processorsManager->addProcessor(processor);
    }

    return processorsManager->getAllProcessorsInfo();
}
