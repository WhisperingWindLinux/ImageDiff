#include "pythonscriptfilter.h"

#include <qbuffer.h>
#include <qprocess.h>

#include <business/pluginsettingsinteractor.h>

#include <domain/valueobjects/comparisonresultvariant.h>

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

QImage PythonScripFilter::filter(QImage image) {
    QByteArray image1Data;

    QBuffer buffer1(&image1Data);

    buffer1.open(QIODevice::WriteOnly);

    if (!image.save(&buffer1, "PNG")) {
        throw runtime_error("Failed to encode images to bytes array.");
    }

    auto pluginSettings = PluginSettingsInteractor().getPluginSettings();
    if (pluginSettings.pythonInterpreterPath.isEmpty()) {
        throw runtime_error("Bad python interpreter.");
    }

    QProcess process;
    process.start(pluginSettings.pythonInterpreterPath, QStringList() << pyScriptPath);

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
    if (!resultImage.loadFromData(output, "PNG")) {
        throw runtime_error("Failed to load the image from data.");
    }

    if (resultImage.isNull()) {
        throw runtime_error("The Python script returns an empty image.");
    };

    return resultImage;
}
