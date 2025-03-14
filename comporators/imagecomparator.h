#ifndef IMAGECOMPARATOR_H
#define IMAGECOMPARATOR_H

#include <qimage.h>
#include <qstring.h>

#include <interfaces/comporator.h>


class ImageComparator : public IComparator
{
public:

    ImageComparator() = default;
    virtual ~ImageComparator() = default;

    // AComparator interface

    QString name() override;
    QString hotkey() override;
    QString description() override;
    std::shared_ptr<ComparisonResultVariant> compare(ComparableImage first, ComparableImage second) override;

private:
    QImage compareImages(QImage image1, QImage image2);
};

#endif // IMAGECOMPARATOR_H
