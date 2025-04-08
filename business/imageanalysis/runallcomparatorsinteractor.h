#ifndef RUNALLCOMPARATORSINTERACTOR_H
#define RUNALLCOMPARATORSINTERACTOR_H

#include <domain/valueobjects/autocomparisonreportentry.h>
#include <domain/valueobjects/comparableimage.h>
#include <domain/valueobjects/comparisonresultvariant.h>
#include <domain/valueobjects/images.h>

class IProgressDialog;

class RunAllComparatorsInteractor
{
public:
    RunAllComparatorsInteractor(IProgressDialog *callback,
                                const ComparableImage &firstImage,
                                const ComparableImage &secondImage,
                                const QString &reportDirPath
                                );

    void run();
private:
    IProgressDialog *mCallback;
    ComparableImage mFirstImage;
    ComparableImage mSecondImage;
    QString mReportDirPath;

    QList<AutocomparisonReportEntry> executeAllComparators();
    void generateReports(QList<AutocomparisonReportEntry> &entries);
};

#endif // RUNALLCOMPARATORSINTERACTOR_H
