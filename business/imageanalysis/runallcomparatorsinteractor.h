#ifndef RUNALLCOMPARATORSINTERACTOR_H
#define RUNALLCOMPARATORSINTERACTOR_H

#include <domain/valueobjects/autocomparisonreportentry.h>
#include <domain/valueobjects/comparableimage.h>
#include <domain/valueobjects/comparisonresultvariant.h>

using namespace std;

class IProgressDialog;

class RunAllComparatorsInteractor
{
public:
    RunAllComparatorsInteractor(IProgressDialog *progressDialog,
                                ComparableImage firstImage,
                                ComparableImage secondImage,
                                QString reportDirPath
                                );
    void run();

private:
    IProgressDialog *progressDialog;
    ComparableImage firstImage;
    ComparableImage secondImage;
    QString reportDirPath;

    QList<AutocomparisonReportEntry> executeAllComparators();
    void generateReports(QList<AutocomparisonReportEntry> entries);
};

#endif // RUNALLCOMPARATORSINTERACTOR_H
