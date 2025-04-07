#ifndef COMPARISONRESULTVARIANT_H
#define COMPARISONRESULTVARIANT_H

#include <QtGui/qimage.h>

enum class ComparisonResultVariantType { None, Image, String };

class ComparisonResultVariant
{
public:
    ComparisonResultVariant();
    ComparisonResultVariant(const QImage &image);
    ComparisonResultVariant(const QString &string);

    QImage getImageResult();
    QString getStringResult();

    ComparisonResultVariantType getType();

    friend class TestComparisonResultVariant;

private:
    ComparisonResultVariantType mType { ComparisonResultVariantType::None };
    QImage mImage;
    QString mString;
};

typedef std::shared_ptr<ComparisonResultVariant> ComparisonResultVariantPtr;

#endif // COMPARISONRESULTVARIANT_H
