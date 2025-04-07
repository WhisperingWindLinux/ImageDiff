#include "autocomparisonreportentry.h"

#include "comparisonresultvariant.h"

AutocomparisonReportEntry::AutocomparisonReportEntry(std::shared_ptr<ComparisonResultVariant> result,
                                                     std::optional<ImageProcessorInfo> imageProcessorInfo)
{
    if (result == nullptr) {
        return;
    }
    if (result->getType() == ComparisonResultVariantType::String) {
        mTextReport = result->getStringResult();
    } else if (result->getType() == ComparisonResultVariantType::Image) {
        mImageReport = result->getImageResult();
    }
    this->mImageProcessorInfo = imageProcessorInfo;
}

std::optional<QString> AutocomparisonReportEntry::getTextReport() const {
    return mTextReport;
}

std::optional<QImage> AutocomparisonReportEntry::getImagereport() const {
    return mImageReport;
}

std::optional<ImageProcessorInfo> AutocomparisonReportEntry::getImageProcessorInfo() const {
    return mImageProcessorInfo;
}
