#pragma once

#include <qpixmap.h>

#include <domain/interfaces/filter.h>

class GrayscaleFilter : public IFilter
{
public:
    GrayscaleFilter() = default;
    virtual ~GrayscaleFilter() = default;

// ATransformer interface

public:
    QString getShortName() const override;
    QString getHotkey() const override;
    QString getDescription() const override;
    QImage filter(QImage image) override;
};
