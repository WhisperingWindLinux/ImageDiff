#ifndef COMPARISIONRESULTVARIANT_H
#define COMPARISIONRESULTVARIANT_H

#include <QtGui/qimage.h>

enum class ComparisionResultVariantType { None, Image, String };

class ComparisionResultVariant
{
public:
    ComparisionResultVariant();
    ComparisionResultVariant(QImage image);
    ComparisionResultVariant(QString string);

    QImage imageResult();
    QString stringResult();

    ComparisionResultVariantType type();

private:
    ComparisionResultVariantType _type { ComparisionResultVariantType::None };
    QImage image;
    QString string;
};

#endif // COMPARISIONRESULTVARIANT_H
