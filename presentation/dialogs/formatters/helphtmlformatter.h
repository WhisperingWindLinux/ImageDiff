#ifndef HELPHTMLFORMATTER_H
#define HELPHTMLFORMATTER_H


#include <QString>
#include <QList>
#include <QStringList>

#include <domain/valueobjects/imageprocessorsinfo.h>

class HelpHtmlFormatter {
public:
    HelpHtmlFormatter() = delete;
    ~HelpHtmlFormatter() = delete;
    
    static QString formatImageProcessorInfo(const ImageProcessorInfo &info);

private:
    static QString enumToString(ImageProcessorType type);
    static QString formatProperty(const Property &property);
};

#endif // HELPHTMLFORMATTER_H
