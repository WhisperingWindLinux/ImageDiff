#ifndef IPROCESSORPROPERTIESDIALOGCALLBACK_H
#define IPROCESSORPROPERTIESDIALOGCALLBACK_H

#include <domain/valueobjects/property.h>

class IPropcessorPropertiesDialogCallback {
public:
    virtual QList<Property> showImageProcessorPropertiesDialog(const QString &processorName,
                                                               const QString &processorDescription,
                                                               const QList<Property> &defaultProperties) = 0;
};

#endif // IPROCESSORPROPERTIESDIALOGCALLBACK_H
