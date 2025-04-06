#ifndef IMAGEPROCESSORDESERIALIZER_H
#define IMAGEPROCESSORDESERIALIZER_H

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QDebug>
#include <memory>

#include <domain/valueobjects/pyscriptinfo.h>
#include <domain/interfaces/business/icomparator.h>
#include <domain/interfaces/business/ifilter.h>
#include <domain/interfaces/business/imageprocessor.h>

class ImageProcessorDeserializer {
public:
    ImageProcessorDeserializer(const QList<PyScriptInfo> &pyScriptsInfo);
    QList<std::shared_ptr<IImageProcessor>> deserialize();

private:
    const QList<PyScriptInfo> pyScriptsInfo;

    std::shared_ptr<IFilter> createFilter(const QJsonObject& obj, QString pyScriptPath);
    std::shared_ptr<IComparator> createComparator(const QJsonObject& obj, QString pyScriptPath);
    QList<Property> parseProperties(const QJsonArray& array);
};

#endif // IMAGEPROCESSORDESERIALIZER_H
