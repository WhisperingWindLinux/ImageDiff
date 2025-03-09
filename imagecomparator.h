#ifndef IMAGECOMPARATOR_H
#define IMAGECOMPARATOR_H

#include <qimage.h>
#include <qstring.h>

#include <interfaces/acomporator.h>


class ImageComparator : public AComparator
{
public:

    // AComparator interface

    QString name();
    QString hotkey();
    QString description();
    std::shared_ptr<ComparisionResultVariant> compare(QList<QString> filesPath);
    virtual ComporatorContentType contentType();

private:
    QImage compareImages(const QString imagePath1, const QString imagePath2);
};

#endif // IMAGECOMPARATOR_H
