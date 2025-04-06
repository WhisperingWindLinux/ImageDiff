#include "otherappinstancesinteractor.h"
#include "business/utils/imagesinfo.h"
#include "domain/interfaces/presentation/iotherappinstancesinteractorcallback.h"

#include <qcoreapplication.h>
#include <qdir.h>
#include <qprocess.h>

#include <business/validation/imagevalidationrulesfactory.h>

OtherAppInstancesInteractor::OtherAppInstancesInteractor(
                                                OtherAppInstancesInteractorCallback *callback)
    : callback(callback)
{
}

OtherAppInstancesInteractor::~OtherAppInstancesInteractor() {
    for (auto it = appInstances.begin(); it != appInstances.end(); ) {
        (*it)->deleteLater();
    }
    appInstances.clear();
}

void OtherAppInstancesInteractor::cleanupProcesses() {
    for (auto it = appInstances.begin(); it != appInstances.end(); ) {
        QProcess* process = *it;
        if (process->state() == QProcess::NotRunning) {
            process->deleteLater();
            it = appInstances.erase(it);
        } else {
            ++it;
        }
    }
}

void OtherAppInstancesInteractor::openNewAppInstance(ImagesPtr images) {

    ImagesInfo info { images };

    std::optional<QString> path1 = saveImageInTempDir(images->image1, info.getFirstImageBaseName());
    std::optional<QString> path2;

    if (!images->isTheSameImage()) {
        path2 = saveImageInTempDir(images->image2, info.getSecondImageBaseName());
    }

    if (path1 && (images->isTheSameImage() || path2)) {
        openNewAppInstance(path1.value(), path2);
        return;
    }
    if (path1) {
        QFile(path1.value()).remove();
    }
    if (path2) {
        QFile(path2.value()).remove();
    }
}

void OtherAppInstancesInteractor::openNewAppInstance(const QString &firstFilePath,
                                                     const std::optional<QString> &secondFilePath
                                                     )
{
    cleanupProcesses();

    auto process = new QProcess();
    appInstances.append(process);

    QString program = QCoreApplication::applicationFilePath();
    QStringList arguments;
    arguments << firstFilePath;
    if (secondFilePath) {
        arguments << secondFilePath.value();
    }

    if (process->startDetached(program, arguments)) {
        callback->onOtherAppInstanceOpened();
    } else {
        callback->showError("Failed to start the new instance of ImageDiff: " + process->errorString());
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
