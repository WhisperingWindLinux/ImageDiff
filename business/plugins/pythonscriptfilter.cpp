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
    : shortName(shortName),
    hotkey(hotkey),
    description(description),
    properties(properties),
    pyScriptPath(pyScriptPath)
{
    auto validationRules = ImageValidationRulesFactory::createImageExtentionValidator();
    QString ext = validationRules->getDeafaultSaveExtention(false);
    defaultSaveImageExtention = ext.toUpper().toStdString();
}

QString PythonScripFilter::getShortName() const {
    return shortName;
}

QString PythonScripFilter::getHotkey() const {
    return hotkey;
}

QString PythonScripFilter::getDescription() const {
    return description;
}

QList<Property> PythonScripFilter::getDefaultProperties() const {
    return properties;
}

void PythonScripFilter::setProperties(QList<Property> properties) {
    this->properties = properties;
}

QImage PythonScripFilter::filter(const QImage &image) {
    QByteArray image1Data;

    QBuffer buffer1(&image1Data);

    buffer1.open(QIODevice::WriteOnly);

    if (!image.save(&buffer1, defaultSaveImageExtention.c_str())) {
        throw runtime_error("Failed to encode images to bytes array.");
    }

    auto pluginSettings = PluginsSettingsInteractor().getPluginSettings();
    if (pluginSettings.pythonInterpreterPath.isEmpty()) {
        throw runtime_error("Bad python interpreter.");
    }

    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("Runner", "ImageDiff");
    process.setProcessEnvironment(env);


    QStringList params;
    params << pyScriptPath;

    foreach (auto property, properties) {
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
    if (!resultImage.loadFromData(output, defaultSaveImageExtention.c_str()) &&
        !resultImage.isNull()) {
        throw runtime_error("Error! The script returned '" +
                            process.readAllStandardError() + "'");
    }

    return resultImage;
}
