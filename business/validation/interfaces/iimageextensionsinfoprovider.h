#ifndef IIMAGEEXTENSIONSINFOPROVIDER_H
#define IIMAGEEXTENSIONSINFOPROVIDER_H

#include <qstring.h>


class IImageExtensionsInfoProvider
{
public:
    virtual QString getDeafaultSaveExtension(bool includeDot) = 0;
    virtual QString createOpenFilter() = 0;
    virtual QString createSaveFilter() = 0;
};


#endif // IIMAGEEXTENSIONSINFOPROVIDER_H
