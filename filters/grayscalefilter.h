#pragma once

#include <qpixmap.h>

#include <imageprocessorsmanager/property.h>

#include <interfaces/filter.h>


class GrayscaleFilter : public IFilter
{
public:
    GrayscaleFilter() = default;
    virtual ~GrayscaleFilter() = default;

// ATransformer interface

public:
    QString name() override;
    QString hotkey() override;
    QString description() override;
    QImage filter(QImage image) override;
};
