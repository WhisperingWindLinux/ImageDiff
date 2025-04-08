#include "otherappinstancesinteractor.h"

#include <qcoreapplication.h>
#include <qdir.h>
#include <qprocess.h>

#include <business/validation/imagevalidationrulesfactory.h>
#include "business/utils/imagesinfo.h"
#include "domain/interfaces/presentation/iotherappinstancesinteractorcallback.h"


OtherAppInstancesInteractor::OtherAppInstancesInteractor(
                                                OtherAppInstancesInteractorCallback *callback)
    : mCallback(callback)
{
}

OtherAppInstancesInteractor::~OtherAppInstancesInteractor() {
    for (auto it = mAppInstances.begin(); it != mAppInstances.end(); ) {
        (*it)->deleteLater();
    }
    mAppInstances.clear();
}

void OtherAppInstancesInteractor::cleanupProcesses() {
    for (auto it = mAppInstances.begin(); it != mAppInstances.end(); ) {
        QProcess* process = *it;
        if (process->state() == QProcess::NotRunning) {
            process->deleteLater();
            it = mAppInstances.erase(it);
        } else {
            ++it;
        }
    }
}

void OtherAppInstancesInteractor::openNewAppInstance(ImageHolderPtr images) {

    ImagesInfo info { images };

    std::optional<QString> firstImagePath = saveImageInTempDir(images->getFirstImage(),
                                                               info.getFirstImageBaseName()
                                                               );
    std::optional<QString> secondImagePath;

    if (!images->isSingleImage()) {
        secondImagePath = saveImageInTempDir(images->getSecondImage(), info.getSecondImageBaseName());
    }

    if (firstImagePath && (images->isSingleImage() || secondImagePath)) {
        coreOpenNewAppInstance(firstImagePath.value(), secondImagePath);
        return;
    }
    if (firstImagePath) {
        QFile(firstImagePath.value()).remove();
    }
    if (secondImagePath) {
        QFile(secondImagePath.value()).remove();
    }
}

void OtherAppInstancesInteractor::coreOpenNewAppInstance(const QString &firstFilePath,
                                                         const std::optional<QString> &secondFilePath
                                                         )
{
    cleanupProcesses();

    auto process = new QProcess();
    mAppInstances.append(process);

    QString program = QCoreApplication::applicationFilePath();
    QStringList arguments;
    arguments << firstFilePath;
    if (secondFilePath) {
        arguments << secondFilePath.value();
    }

    if (process->startDetached(program, arguments)) {
        mCallback->onOtherAppInstanceOpened();
    } else {
        mCallback->showError("Failed to start the new instance of TwinPix: " + process->errorString());
    }
}

std::optional<QString> OtherAppInstancesInteractor::saveImageInTempDir(const QPixmap &pixmap,
                                                                       const QString &fileName
                                                                       )
{
    QString tempDir = QDir::tempPath();
    auto validationRules = ImageValidationRulesFactory::createImageExtensionsInfoProvider();
    auto ext = validationRules->getDeafaultSaveExtension(true);
    QString filePath = QDir(tempDir).filePath(QString(fileName) + ext);
    if (pixmap.save(filePath)) {
        return filePath;
    } else {
        return std::nullopt;
    }
}
