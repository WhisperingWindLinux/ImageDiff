#ifndef AUTOCOMPARISONREPORTENTRY_H
#define AUTOCOMPARISONREPORTENTRY_H

#include "comparisonresultvariant.h"
#include "imageprocessorsinfo.h"

#include <qimage.h>
#include <qstring.h>

class AutocomparisonReportEntry {
public:
    AutocomparisonReportEntry(std::shared_ptr<ComparisonResultVariant> result,
                              std::optional<ImageProcessorInfo> imageProcessorInfo
                              );

    std::optional<QString> getTextReport() const;
    std::optional<QImage> getImagereport() const;
    std::optional<ImageProcessorInfo> getImageProcessorInfo() const;

private:
    std::optional<QString> mTextReport;
    std::optional<QImage> mImageReport;
    std::optional<ImageProcessorInfo> mImageProcessorInfo;
};


#endif // AUTOCOMPARISONREPORTENTRY_H
