#ifndef IMAGEEXTENTIONVALIDATOR_H
#define IMAGEEXTENTIONVALIDATOR_H

#include <qstring.h>
#include <qlist>

#include <business/validation/interfaces/iimageextentionvalidator.h>


class ImageExtentionValidator : public IImageExtentionValidator
{
public:
    ImageExtentionValidator();
    virtual ~ImageExtentionValidator() = default;

    bool canOpen(const QString &imagePath) override;
    QString getAllExtentionsForUserMessages(bool isToUpper) override;
    QString getDeafaultSaveExtention(bool includeDot = false) override;
    QString createFilter() override;

private:
    QList<QString> extsForOpen;
    QString extForSave;
};

#endif // IMAGEEXTENTIONVALIDATOR_H
