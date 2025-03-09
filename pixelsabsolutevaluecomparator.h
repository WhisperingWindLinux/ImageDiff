#ifndef PIXELSABSOLUTEVALUECOMPARATOR_H
#define PIXELSABSOLUTEVALUECOMPARATOR_H

#include <QPair>

#include <interfaces/acomporator.h>

class PixelsAbsoluteValueComparator : public AComparator
{
public:

    // AComparator interface

    QString name();
    QString hotkey();
    QString description();
    std::shared_ptr<ComparisionResultVariant> compare(QList<QString> filesPath);
    virtual ComporatorContentType contentType();

private:
    // Function to calculate pixel differences between two images
    QMap<QPair<int, int>, QPair<int, double>> compareImages(const QString image1Path, const QString image2Path);

    // Function to format the comparison result as an HTML string
    static QString formatResultToHtml(const QMap<QPair<int, int>, QPair<int, double>>& result);
};

#endif // PIXELSABSOLUTEVALUECOMPARATOR_H
