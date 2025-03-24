#ifndef IMAGEPROCESSORSINFO_H
#define IMAGEPROCESSORSINFO_H

#include "domain/interfaces/imageprocessor.h"
#include <qstring.h>

struct ImageProcessorInfo {

    ImageProcessorInfo() {}

    ImageProcessorInfo(const QString &name,
                       const QString &fullName,
                       const QString &description,
                       const QString &hotkey,
                       ImageProcessorType type,
                       QList<Property> properties)
        : name(name), fullName(fullName), description(description),
        hotkey(hotkey), type(type), properties(properties) {}

    ImageProcessorInfo(const ImageProcessorInfo &other)
        : name(other.name),
        fullName(other.fullName),
        description(other.description),
        hotkey(other.hotkey),
        type(other.type),
        properties(other.properties) {}

    ImageProcessorInfo(ImageProcessorInfo &&other) noexcept
        : name(std::move(other.name)),
        fullName(std::move(other.fullName)),
        description(std::move(other.description)),
        hotkey(std::move(other.hotkey)),
        type(std::move(other.type)),
        properties(std::move(other.properties)) {}

    ImageProcessorInfo& operator=(const ImageProcessorInfo &other) {
        if (this != &other) {
            name = other.name;
            fullName = other.fullName;
            description = other.description;
            hotkey = other.hotkey;
            type = other.type;
            properties = other.properties;
        }
        return *this;
    }

    ImageProcessorInfo& operator=(ImageProcessorInfo &&other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            fullName = std::move(other.fullName);
            description = std::move(other.description);
            hotkey = std::move(other.hotkey);
            type = std::move(other.type);
            properties = std::move(other.properties);
        }
        return *this;
    }

    QString name;
    QString fullName;
    QString description;
    QString hotkey;
    ImageProcessorType type;
    QList<Property> properties;
};


#endif // IMAGEPROCESSORSINFO_H
