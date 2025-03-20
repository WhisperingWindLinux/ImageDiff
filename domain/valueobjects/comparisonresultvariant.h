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

    QImage imageResult();
    QString stringResult();

    ComparisonResultVariantType type();

    friend class TestComparisonResultVariant;

private:
    ComparisonResultVariantType _type { ComparisonResultVariantType::None };
    QImage image;
    QString string;
};

typedef std::shared_ptr<ComparisonResultVariant> ComparisonResultVariantPtr;

#endif // COMPARISONRESULTVARIANT_H
