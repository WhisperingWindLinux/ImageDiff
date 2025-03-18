#include "pythonscriptcomparator.h"

#include <QBuffer>
#include <QtGui/qpixmap.h>
#include <qprocess.h>
#include <QtCore/qdebug.h>
#include <business/pluginsettingsinteractor.h>


PythonScriptComparator::PythonScriptComparator(const QString &pyScriptPath,
                                               const QString &shortName,
                                               const QString &hotkey,
                                               const QString &description,
                                               const QList<Property> &properties,
                                               const QString &fullName,
                                               bool isPartOfAutoReportingToolbox
                                                )
    : shortName(shortName),
    hotkey(hotkey),
    description(description),
    properties(properties),
    fullName(fullName),
    m_isPartOfAutoReportingToolbox(isPartOfAutoReportingToolbox),
    pyScriptPath(pyScriptPath)
{

}

QString PythonScriptComparator::getShortName() const {
    return shortName;
}

QString PythonScriptComparator::getHotkey() const {
    return hotkey;
}

QString PythonScriptComparator::getDescription() const {
    return description;
}

QList<Property> PythonScriptComparator::getDefaultProperties() const {
    return properties;
}

void PythonScriptComparator::setProperties(QList<Property> properties) {
    this->properties = properties;
}

QString PythonScriptComparator::getFullName() const {
    return fullName;
}

bool PythonScriptComparator::isPartOfAutoReportingToolbox() {
    return m_isPartOfAutoReportingToolbox;
}

shared_ptr<ComparisonResultVariant> PythonScriptComparator::compare(ComparableImage first,
                                                                    ComparableImage second
                                                                    )
{
    QByteArray image1Data;
    QByteArray image2Data;

    QBuffer buffer1(&image1Data);
    QBuffer buffer2(&image2Data);

    buffer1.open(QIODevice::WriteOnly);
    buffer2.open(QIODevice::WriteOnly);

    if (!first.getImage().save(&buffer1, "PNG") || !second.getImage().save(&buffer2, "PNG")) {
        throw runtime_error("Failed to encode images to bytes array.");
    }

    auto pluginSettings = PluginsSettingsInteractor().getPluginSettings();
    if (pluginSettings.pythonInterpreterPath.isEmpty()) {
        throw runtime_error("Bad python interpreter.");
    }

    QProcess process;
    process.start(pluginSettings.pythonInterpreterPath, QStringList()
                                                            << pyScriptPath
                                                            << first.getName()
                                                            << second.getName());

    if (!process.waitForStarted()) {
        throw runtime_error("Failed to start process.");
    }

    QByteArray inputData;

    int size1 = image1Data.size();
    inputData.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&size1),
                                             sizeof(size1))
                     );
    inputData.append(image1Data);

    int size2 = image2Data.size();
    inputData.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&size2),
                                             sizeof(size2))
                     );
    inputData.append(image2Data);process.write(inputData);
    process.closeWriteChannel();

    if (!process.waitForFinished()) {
        auto errorMsg =  QString("Process error: ") + process.errorString();
        throw runtime_error(errorMsg.toStdString());
    }

    QByteArray output = process.readAllStandardOutput();
    QImage resultImage;
    if (!resultImage.loadFromData(output, "PNG")) {
        qDebug() << process.readAllStandardError();
        throw runtime_error("Failed to load the image from data.");
    }

    if (resultImage.isNull()) {
        throw runtime_error("The Python script returns an empty image.");
    };

    return make_shared<ComparisonResultVariant>(resultImage);
}
