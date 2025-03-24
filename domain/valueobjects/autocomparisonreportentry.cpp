#include "autocomparisonreportentry.h"

#include "comparisonresultvariant.h"

AutocomparisonReportEntry::AutocomparisonReportEntry(std::shared_ptr<ComparisonResultVariant> result,
                                                     std::optional<ImageProcessorInfo> imageProcessorInfo)
{
    if (result == nullptr) {
        return;
    }
    if (result->type() == ComparisonResultVariantType::String) {
        textReport = result->stringResult();
    } else if (result->type() == ComparisonResultVariantType::Image) {
        imageReport = result->imageResult();
    }
    this->imageProcessorInfo = imageProcessorInfo;
}

std::optional<QString> AutocomparisonReportEntry::getTextReport() const {
    return textReport;
}

std::optional<QImage> AutocomparisonReportEntry::getImagereport() const {
    return imageReport;
}

std::optional<ImageProcessorInfo> AutocomparisonReportEntry::getImageProcessorInfo() const {
    return imageProcessorInfo;
}
