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
    QString name() const override;
    QString hotkey() const override;
    QString htmlFormattedHelp() const override;
    QImage filter(QImage image) override;
};
