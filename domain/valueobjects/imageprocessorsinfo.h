#ifndef IMAGEPROCESSORSINFO_H
#define IMAGEPROCESSORSINFO_H

#include "domain/interfaces/imageprocessor.h"
#include <qstring.h>

struct ImageProcessorInfo {

    ImageProcessorInfo(const QString &name,
                       const QString &description,
                       const QString &hotkey,
                       ImageProcessorType type)
        : name(name), description(description),
        hotkey(hotkey), type(type) {}

    ImageProcessorInfo(const ImageProcessorInfo &other)
        : name(other.name),
        description(other.description),
        hotkey(other.hotkey),
        type(other.type) {}

    ImageProcessorInfo(ImageProcessorInfo &&other) noexcept
        : name(std::move(other.name)),
        description(std::move(other.description)),
        hotkey(std::move(other.hotkey)),
        type(std::move(other.type)) {}

    ImageProcessorInfo& operator=(const ImageProcessorInfo &other) {
        if (this != &other) {
            name = other.name;
            description = other.description;
            hotkey = other.hotkey;
            type = other.type;
        }
        return *this;
    }

    ImageProcessorInfo& operator=(ImageProcessorInfo &&other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            description = std::move(other.description);
            hotkey = std::move(other.hotkey);
            type = std::move(other.type);
        }
        return *this;
    }

    QString name;
    QString description;
    QString hotkey;
    ImageProcessorType type;
};


#endif // IMAGEPROCESSORSINFO_H
