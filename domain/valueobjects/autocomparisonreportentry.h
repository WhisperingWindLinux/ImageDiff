#ifndef AUTOCOMPARISONREPORTENTRY_H
#define AUTOCOMPARISONREPORTENTRY_H

#include "comparisonresultvariant.h"

#include <qimage.h>
#include <qstring.h>

class AutocomparisonReportEntry {
public:
    AutocomparisonReportEntry(std::shared_ptr<ComparisonResultVariant> result,
                              const QString &comporatorFullName,
                              const QString &comporatorDescription
                              );

    std::optional<QString> getTextReport() const;

    std::optional<QImage> getImagereport() const;

    QString getComporatorFullName() const;

    QString getComporatorDescription() const;

private:
    std::optional<QString> textReport;
    std::optional<QImage> imageReport;
    QString comporatorFullName;
    QString comporatorDescription;
};


#endif // AUTOCOMPARISONREPORTENTRY_H
