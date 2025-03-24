#ifndef IMAGEEXTENSIONVALIDATOR_H
#define IMAGEEXTENSIONVALIDATOR_H

#include <qstring.h>
#include <qlist>

#include <business/validation/interfaces/iimageextentionvalidator.h>


class ImageExtensionValidator : public IImageExtentionValidator
{
public:
    ImageExtensionValidator();
    virtual ~ImageExtensionValidator() = default;

    bool canOpen(const QString &imagePath) override;
    QString getAllExtensionsForUserMessages(bool isToUpper) override;
    QString getDeafaultSaveExtension(bool includeDot = false) override;
    QString createFilter() override;

private:
    QList<QString> extensionsForOpen;
    QString extensionForSave;
};

#endif // IMAGEEXTENSIONVALIDATOR_H
