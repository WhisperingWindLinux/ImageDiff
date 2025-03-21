#include "imageprocessinginteractor.h"

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
#include <domain/interfaces/imageprocessinginteractorlistener.h>
#include <business/utils/imagesinfo.h>
#include "domain/interfaces/processorpropertiesdialogcallback.h"
#include "imageprocessorsmanager.h"
#include "runallcomparatorsinteractor.h"

ImageProcessingInteractor::ImageProcessingInteractor(
                                    const ImagesPtr images,
                                    IPropcessorPropertiesDialogCallback *propertiesDialogCallback,
                                    IProgressDialog *progressDialogCallback
                                        )
    : propertiesDialogCallback(propertiesDialogCallback),
    progressDialogCallback(progressDialogCallback),
    originalImages(images),
    displayedImages(images)
{
}

ImageProcessingInteractor::~ImageProcessingInteractor() {
    listeners.clear();
}

void ImageProcessingInteractor::callImageProcessor(const QVariant &callerData) {
    try {
        coreCallImageProcessor(callerData);
    } catch(std::runtime_error &e) {
        notifyImageProcessorFailed(e.what());
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}

void ImageProcessingInteractor::coreCallImageProcessor(const QVariant &callerData) {
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

void ImageProcessingInteractor::restoreOriginalImages() {
    if (originalImages == nullptr) {
        return;
    }
    displayedImages = originalImages;
    notifyFilteredResultLoaded(displayedImages->image1, displayedImages->image2);
}

void ImageProcessingInteractor::showImageProcessorsHelp() {
    auto processorsInfo = ImageProcessorsManager::instance()->getAllProcessorsInfo();
    if (processorsInfo.size() == 0) {
        return;
    }
    auto helpHtml = HtmlImageProcessorsHelpPresenter::formatToHTML(processorsInfo);
    notifyShowImageProcessorsHelp(helpHtml);
}

void ImageProcessingInteractor::handleProcessorPropertiesIfNeed(IImageProcessorPtr processor) {
    auto properties = processor->getDefaultProperties();
    if (properties.empty()) {
        return;
    }
    auto newProperties = propertiesDialogCallback->showImageProcessorPropertiesDialog(
                                                                    processor->getShortName(),
                                                                    processor->getDescription(),
                                                                    properties
                                                                );
    if (!newProperties.empty()) {
        processor->setProperties(newProperties);
    }
}

void ImageProcessingInteractor::callComparator(IComparatorPtr comparator) {
    if (displayedImages == nullptr) {
        return;
    }
    ImagesInfo info { displayedImages };

    auto firstImage = displayedImages->image1;
    auto firstImagePath = displayedImages->path1;
    auto secondImage = displayedImages->image2;
    auto secondImagePath = displayedImages->path2;
    auto firstImageName = info.getFirstImageName();
    auto secondImageName = info.getSecondImageName();

    ComparableImage comapableImage1 { firstImage, firstImageName };
    ComparableImage comapableImage2 { secondImage, secondImageName };

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
        notifyComparisonResultLoaded(pixmap, comparator->getShortName());
    }
    else if (result->type() == ComparisonResultVariantType::String) {
        QString stringResult = result->stringResult();
        if (stringResult.isNull() || stringResult.isEmpty()) {
            throw std::runtime_error("Error: The comparator returns an empty result.");
        }
        notifyComparisonResultLoaded(stringResult,
                                     comparator->getFullName(),
                                     comparator->getDescription(),
                                     firstImagePath,
                                     secondImagePath
                                    );
    }
}

void ImageProcessingInteractor::callFilter(IFilterPtr filter) {
    if (displayedImages == nullptr) {
        return;
    }
    auto firstImage = displayedImages->image1.toImage();
    auto secondImage = displayedImages->image2.toImage();

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

    notifyFilteredResultLoaded(pixmap1, pixmap2);
}

QList<ImageProcessorInfo> ImageProcessingInteractor::getImageProcessorsInfo() {

    PluginsManager *pluginsManager = new PluginsManager();
    ImageProcessorsManager *processorsManager = ImageProcessorsManager::instance();
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

void ImageProcessingInteractor::runAllComparators() {

    ImagesInfo info { displayedImages };

    auto image1 = displayedImages->image1;
    auto image2 = displayedImages->image2;
    auto name1 = info.getFirstImageBaseName();
    auto name2 = info.getSecondImageBaseName();
    auto path1 = info.getFirstImagePath();
    auto path2 = info.getSecondImagePath();

    QString reportDirName = QString("%1_vs_%2_comparison_report").arg(name1, name2);

    QString saveReportDirPath = info.getFirstImageDir() + QDir::separator() + reportDirName;

    ComparableImage firstComparableImage {image1, path1};
    ComparableImage secondComparableImage {image2, path2};

    RunAllComparatorsInteractor runAllComparatorsInteractor {
                                            progressDialogCallback,
                                            firstComparableImage,
                                            secondComparableImage,
                                            saveReportDirPath
                                        };

    runAllComparatorsInteractor.run();
}

bool ImageProcessingInteractor::subscribe(IImageProcessingInteractorListener *listener) {
    if (listener == nullptr) {
        return false;
    }
    if (listeners.contains(listener)) {
        return false;
    }
    listeners.append(listener);
    return true;
}

bool ImageProcessingInteractor::unsubscribe(const IImageProcessingInteractorListener *listener) {
    if (listener == nullptr) {
        return false;
    }
    return listeners.removeOne(listener);
}

void ImageProcessingInteractor::notifyShowImageProcessorsHelp(const QString &html) {
    foreach (auto listener, listeners) {
        listener->onShowImageProcessorsHelp(html);
    }
}

void ImageProcessingInteractor::notifyComparisonResultLoaded(const QPixmap &image,
                                                             const QString &description
                                                             )
{
    foreach (auto listener, listeners) {
        listener->onComparisonResultLoaded(image, description);
    }
}

// TBD Refactoring
void ImageProcessingInteractor::notifyComparisonResultLoaded(const QString &html,
                                                             const QString &comporatorFullName,
                                                             const QString &comporatorDescription,
                                                             const QString &firstImagePath,
                                                             const QString &secondImagePath
                                                            )
{
    foreach (auto listener, listeners) {
        listener->onComparisonResultLoaded(html,
                                           comporatorFullName,
                                           comporatorDescription,
                                           firstImagePath,
                                           secondImagePath
                                           );
    }
}

void ImageProcessingInteractor::notifyFilteredResultLoaded(const QPixmap &firstImage,
                                                           const QPixmap &secondImage
                                                           )
{
    foreach (auto listener, listeners) {
        listener->onFilteredResultLoaded(firstImage, secondImage);
    }
}

void ImageProcessingInteractor::notifyImageProcessorFailed(const QString &error) {
    foreach (auto listener, listeners) {
        listener->onImageProcessorFailed(error);
    }
}
