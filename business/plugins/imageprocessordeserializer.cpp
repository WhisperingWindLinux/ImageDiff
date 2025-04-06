#include "imageprocessordeserializer.h"

#include "pythonscriptcomparator.h"
#include "pythonscriptfilter.h"

ImageProcessorDeserializer::ImageProcessorDeserializer(const QList<PyScriptInfo> &pyScriptsInfo)
    : pyScriptsInfo(pyScriptsInfo)
{
}

QList<std::shared_ptr<IImageProcessor> > ImageProcessorDeserializer::deserialize()
{
    QList<std::shared_ptr<IImageProcessor>> processors;

    foreach (auto pyScriptInfo, pyScriptsInfo) {
        QFile file(pyScriptInfo.jsonFilePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open file:" << file.fileName();
            continue;
        }
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isObject()) {
            qWarning() << "Invalid JSON format in file";
            continue;
        }
        QJsonObject obj = doc.object();
        QString type = obj.value("type").toString();
        if (type == "Filter") {
            processors.append(createFilter(obj, pyScriptInfo.pythonFilePath));
        } else if (type == "Comparator") {
            processors.append(createComparator(obj, pyScriptInfo.pythonFilePath));
        } else {
            qWarning() << "Unknown processor type";
        }
    }

    return processors;
}

std::shared_ptr<IFilter> ImageProcessorDeserializer::createFilter(const QJsonObject &obj,
                                                                  QString pyScriptPath
                                                                  )
{
    QString shortName = obj.value("shortName").toString();
    QString hotkey = obj.value("hotkey").toString();
    QString description = obj.value("description").toString();
    QList<Property> properties = parseProperties(obj.value("properties").toArray());

    return std::make_shared<PythonScripFilter>(pyScriptPath,
                                               shortName,
                                               hotkey,
                                               description,
                                               properties
                                               );
}

std::shared_ptr<IComparator> ImageProcessorDeserializer::createComparator(const QJsonObject &obj,
                                                                          QString pyScriptPath
                                                                          )
{
    QString shortName = obj.value("shortName").toString();
    QString hotkey = obj.value("hotkey").toString();
    QString description = obj.value("description").toString();
    QList<Property> properties = parseProperties(obj.value("properties").toArray());
    QString fullName = obj.value("fullName").toString();
    bool isPartOfAutoReportingToolbox = obj.value("isPartOfAutoReportingToolbox").toBool();

    return std::make_shared<PythonScriptComparator>(pyScriptPath,
                                                    shortName,
                                                    hotkey,
                                                    description,
                                                    properties,
                                                    fullName,
                                                    isPartOfAutoReportingToolbox
                                                    );
}

QList<Property> ImageProcessorDeserializer::parseProperties(const QJsonArray &array) {
    QList<Property> properties;

    for (const QJsonValue& value : array) {
        QJsonObject propertyObj = value.toObject();
        QString name = propertyObj.value("name").toString();
        QString type = propertyObj.value("type").toString();
        QString description = propertyObj.value("description").toString();

        if (type.compare("Integer", Qt::CaseInsensitive) == 0) {
            int defaultValue = propertyObj.value("defaultValue").toInt();
            int minValue = propertyObj.value("minValue").toInt();
            int maxValue = propertyObj.value("maxValue").toInt();
            properties.append(Property::createIntProperty(name,
                                                          description,
                                                          defaultValue,
                                                          minValue,
                                                          maxValue)
                              );
        } else if (type.compare("Real", Qt::CaseInsensitive) == 0) {
            double defaultValue = propertyObj.value("defaultValue").toDouble();
            double minValue = propertyObj.value("minValue").toDouble();
            double maxValue = propertyObj.value("maxValue").toDouble();
            properties.append(Property::createRealProperty(name,
                                                           description,
                                                           defaultValue,
                                                           minValue,
                                                           maxValue)
                              );
        } else if (type.compare("Alternatives", Qt::CaseInsensitive) == 0) {
            QStringList alternatives;
            foreach (auto alt, propertyObj.value("alternatives").toArray()) {
                alternatives.append(alt.toString());
            }
            int defaultIndex = propertyObj.value("defaultIndex").toInt();
            properties.append(Property::createAlternativesProperty(name,
                                                                   description,
                                                                   alternatives,
                                                                   defaultIndex)
                              );
        } else if (type.compare("FilePath", Qt::CaseInsensitive) == 0) {
            QString defaultValue = propertyObj.value("defaultValue").toString();
            properties.append(Property::createFilePathProperty(name,
                                                               description,
                                                               defaultValue)
                              );
        }
    }

    return properties;
}
