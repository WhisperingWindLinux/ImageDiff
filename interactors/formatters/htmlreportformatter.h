#ifndef HTMLREPORTFORMATTER_H
#define HTMLREPORTFORMATTER_H

#include <qimage.h>
#include <qstring.h>


class HtmlReportFormatter
{
public:

    HtmlReportFormatter() = delete;
    ~HtmlReportFormatter() = delete;

    static bool createReportPage(const QString &folderPath,
                                 const QImage &firstOriginalImage,
                                 const QImage &secondOriginalImage,
                                 const QString &firstImageName,
                                 const QString &secondImageName,
                                 const QList<QPair<QString, QImage> > &comporatorsResults
                                 , const QList<QString> descriptions);
};

#endif // HTMLREPORTFORMATTER_H
