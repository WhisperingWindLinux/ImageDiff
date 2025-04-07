#ifndef IMAGEPROCESSORSINFO_H
#define IMAGEPROCESSORSINFO_H

#include "domain/interfaces/business/imageprocessor.h"
#include <qstring.h>

struct ImageProcessorInfo {

    ImageProcessorInfo() {}

    ImageProcessorInfo(const QString &name,
                       const QString &fullName,
                       const QString &description,
                       const QString &hotkey,
                       ImageProcessorType type,
                       QList<Property> properties
                       )
        : name(name),
        fullName(fullName),
        description(description),
        hotkey(hotkey),
        type(type),
        properties(properties)
    {
    }

    QString name;
    QString fullName;
    QString description;
    QString hotkey;
    ImageProcessorType type;
    QList<Property> properties;
};


#endif // IMAGEPROCESSORSINFO_H
