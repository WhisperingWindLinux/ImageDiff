#include "pythonscriptfilter.h"

#include <qbuffer.h>
#include <qprocess.h>

#include <business/pluginsettingsinteractor.h>

#include <domain/valueobjects/comparisonresultvariant.h>

#include <business/validation/imagevalidationrulesfactory.h>

PythonScripFilter::PythonScripFilter(const QString& pyScriptPath,
                                     const QString &shortName,
                                     const QString &hotkey,
                                     const QString &description,
                                     const QList<Property> &properties)
    : mShortName(shortName),
    mHotkey(hotkey),
    mDescription(description),
    mProperties(properties),
    mPyScriptPath(pyScriptPath)
{
    auto validationRules = ImageValidationRulesFactory::createImageExtensionsInfoProvider();
    QString ext = validationRules->getDeafaultSaveExtension(false);
    mDefaultSaveImageExtention = ext.toUpper().toStdString();
}

QString PythonScripFilter::getShortName() const {
    return mShortName;
}

QString PythonScripFilter::getFullName() const {
    return getShortName();
}

QString PythonScripFilter::getHotkey() const {
    return mHotkey;
}

QString PythonScripFilter::getDescription() const {
    return mDescription;
}

QList<Property> PythonScripFilter::getDefaultProperties() const {
    return mProperties;
}

void PythonScripFilter::setProperties(QList<Property> properties) {
    this->mProperties = properties;
}

QImage PythonScripFilter::filter(const QImage &image) {
    QByteArray image1Data;

    QBuffer buffer1(&image1Data);

    buffer1.open(QIODevice::WriteOnly);

    if (!image.save(&buffer1, mDefaultSaveImageExtention.c_str())) {
        throw runtime_error("Failed to encode images to bytes array.");
    }

    auto pluginSettings = PluginsSettingsInteractor().getPluginSettings();
    if (pluginSettings.pythonInterpreterPath.isEmpty()) {
        throw runtime_error("Bad python interpreter.");
    }

    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("Runner", "TwinPix");
    process.setProcessEnvironment(env);


    QStringList params;
    params << mPyScriptPath;

    foreach (auto property, mProperties) {
        params << property.getAnyValueAsString();
    }

    process.start(pluginSettings.pythonInterpreterPath, params);

    if (!process.waitForStarted()) {
        throw runtime_error("Failed to start process.");
    }

    QByteArray inputData;

    int size1 = image1Data.size();
    inputData.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&size1),
                                             sizeof(size1))
                     );
    inputData.append(image1Data);

    process.write(inputData);
    process.closeWriteChannel();

    if (!process.waitForFinished()) {
        auto errorMsg =  QString("Process error: ") + process.errorString();
        throw runtime_error(errorMsg.toStdString());
    }

    QByteArray output = process.readAllStandardOutput();
    QImage resultImage;
    if (!resultImage.loadFromData(output, mDefaultSaveImageExtention.c_str()) &&
        !resultImage.isNull()) {
        throw runtime_error("Error! The script returned '" +
                            process.readAllStandardError() + "'");
    }

    return resultImage;
}

const QImage& PythonScripFilter::prepareResult(const QImage &resultImage, const QImage &originalImage) {
    int expectedWidth = originalImage.width();
    int expectedHeight = originalImage.height();
    if (resultImage.width() != expectedWidth || resultImage.height() != expectedHeight) {
        QString errorStr = "The resolution of the image obtained from the Filter does not match "
                           "the size of the original image. The Filter must not change "
                           "the original resolution.";
        throw std::runtime_error(errorStr.toStdString());
    } else {
        return resultImage;
    }
}
