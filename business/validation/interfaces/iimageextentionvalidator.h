#ifndef IIMAGEEXTENTIONVALIDATOR_H
#define IIMAGEEXTENTIONVALIDATOR_H

#include <qstring.h>


class IImageExtentionValidator
{
public:
    virtual QString getDeafaultSaveExtension(bool includeDot) = 0;
    virtual QString createOpenFilter() = 0;
    virtual QString createSaveFilter() = 0;
};


#endif // IIMAGEEXTENTIONVALIDATOR_H
