#ifndef IMAGEEXTENSIONVALIDATOR_H
#define IMAGEEXTENSIONVALIDATOR_H

#include <qstring.h>
#include <QList>

#include <business/validation/interfaces/iimageextentionvalidator.h>


class ImageExtensionsInfoProvider : public IImageExtentionValidator
{
public:
    ImageExtensionsInfoProvider();
    virtual ~ImageExtensionsInfoProvider() = default;

    QString getDeafaultSaveExtension(bool includeDot = false) override;
    QString createOpenFilter() override;
    QString createSaveFilter() override;
private:
    QList<QString> extensionsForOpen;
    QString extensionForSave;
};

#endif // IMAGEEXTENSIONVALIDATOR_H
