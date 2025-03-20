#pragma once

#include <qpixmap.h>

#include <domain/interfaces/filter.h>

class GrayscaleFilter : public IFilter
{
public:
    GrayscaleFilter() = default;
    virtual ~GrayscaleFilter() = default;

    // ATransformer interface

    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    QImage filter(const QImage &image) override;
};
