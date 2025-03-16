#ifndef HTMLREPORTPRESENTER_H
#define HTMLREPORTPRESENTER_H

#include <qimage.h>
#include <qstring.h>


class HtmlReportPresenter
{
public:

    HtmlReportPresenter() = delete;
    ~HtmlReportPresenter() = delete;

    static bool createReportPage(const QString &folderPath,
                                 const QImage &firstOriginalImage,
                                 const QImage &secondOriginalImage,
                                 const QString &firstImageName,
                                 const QString &secondImageName,
                                 const QList<QPair<QString, QImage> > &comporatorsResults
                                 , const QList<QString> descriptions);
};

#endif // HTMLREPORTPRESENTER_H
