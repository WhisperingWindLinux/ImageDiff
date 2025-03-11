#pragma once

#include <qpixmap.h>

#include <imageprocessorsmanager/property.h>

#include <interfaces/ATransformer.h>


class GrayscaleTransformer : public ATransformer
{
public:
    GrayscaleTransformer();

// ATransformer interface

public:
    QString name() override;
    QString hotkey() override;
    QString description() override;
    QImage transform(QImage image) override;
};
