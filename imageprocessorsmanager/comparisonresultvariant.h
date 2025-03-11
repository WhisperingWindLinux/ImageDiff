#ifndef COMPARISONRESULTVARIANT_H
#define COMPARISONRESULTVARIANT_H

#include <QtGui/qimage.h>

enum class ComparisonResultVariantType { None, Image, String };

class ComparisonResultVariant
{
public:
    ComparisonResultVariant();
    ComparisonResultVariant(QImage image);
    ComparisonResultVariant(QString string);

    QImage imageResult();
    QString stringResult();

    ComparisonResultVariantType type();

private:
    ComparisonResultVariantType _type { ComparisonResultVariantType::None };
    QImage image;
    QString string;
};

#endif // COMPARISONRESULTVARIANT_H
