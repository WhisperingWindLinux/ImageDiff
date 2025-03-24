#ifndef IIMAGEEXTENTIONVALIDATOR_H
#define IIMAGEEXTENTIONVALIDATOR_H

#include <qstring.h>


class IImageExtentionValidator
{
public:
    virtual bool canOpen(const QString &imagePath) = 0;
    virtual QString getAllExtentionsForUserMessages(bool isToUpper) = 0;
    virtual QString getDeafaultSaveExtention(bool includeDot) = 0;
    virtual QString createFilter() = 0;
};


#endif // IIMAGEEXTENTIONVALIDATOR_H
