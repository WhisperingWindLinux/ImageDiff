#ifndef IMAGEEXTENSIONSINFOPROVIDER_H
#define IMAGEEXTENSIONSINFOPROVIDER_H

#include <qstring.h>
#include <QList>
#include <business/validation/interfaces/iimageextensionsinfoprovider.h>

class ImageExtensionsInfoProvider : public IImageExtensionsInfoProvider
{
public:
    ImageExtensionsInfoProvider();
    virtual ~ImageExtensionsInfoProvider() = default;

    QString getDeafaultSaveExtension(bool includeDot = false) override;
    QString createOpenFilter() override;
    QString createSaveFilter() override;

private:
    QList<QString> mExtensionsForOpen;
    QString mExtensionForSave;
};

#endif // IMAGEEXTENSIONSINFOPROVIDER_H
