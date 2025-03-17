#include "autocomparisonreportentry.h"

#include "comparisonresultvariant.h"

AutocomparisonReportEntry::AutocomparisonReportEntry(std::shared_ptr<ComparisonResultVariant> result,
                                                     const QString &comporatorFullName,
                                                     const QString &comporatorDescription)
    : comporatorFullName(comporatorFullName),
    comporatorDescription(comporatorDescription)
{
    if (result == nullptr) {
        return;
    }
    if (result->type() == ComparisonResultVariantType::String) {
        textReport = result->stringResult();
    } else if (result->type() == ComparisonResultVariantType::Image) {
        imageReport = result->imageResult();
    }
}

std::optional<QString> AutocomparisonReportEntry::getTextReport() const {
    return textReport;
}

std::optional<QImage> AutocomparisonReportEntry::getImagereport() const {
    return imageReport;
}

QString AutocomparisonReportEntry::getComporatorFullName() const {
    return comporatorFullName;
}

QString AutocomparisonReportEntry::getComporatorDescription() const {
    return comporatorDescription;
}
