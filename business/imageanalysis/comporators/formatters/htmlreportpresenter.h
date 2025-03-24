#ifndef HTMLREPORTPRESENTER_H
#define HTMLREPORTPRESENTER_H

#include <qimage.h>
#include <qstring.h>

#include <domain/valueobjects/autocomparisonreportentry.h>
#include <domain/valueobjects/comparableimage.h>


class HtmlReportPresenter
{
public:

    HtmlReportPresenter() = delete;
    ~HtmlReportPresenter() = delete;

    // Writes a comprehensive report consisting of images and text.
    static bool createExtendedReportPage(const QString &folderPath,
                                         const ComparableImage &firstOriginalImage,
                                         const ComparableImage &secondOriginalImage,
                                         QList<AutocomparisonReportEntry> reportEntries
                                         );


    // Writes a simple report consisting only of text.
    static bool createSimpleReportPage(const QString &filePath,
                                       const QString &firstOriginalImageName,
                                       const QString &secondOriginalImageName,
                                       const QString &reportText
                                       );
};

#endif // HTMLREPORTPRESENTER_H
