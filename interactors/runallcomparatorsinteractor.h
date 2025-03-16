#ifndef RUNALLCOMPARATORSINTERACTOR_H
#define RUNALLCOMPARATORSINTERACTOR_H

#include "imageprocessorsmanager/comparableimage.h"

#include <imageprocessorsmanager/comparisonresultvariant.h>

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

    QList<shared_ptr<ComparisonResultVariant> > comparatorsResults;
    QList<QString> comporatorsNames;
    QList<QString> descriptions;

    void executeAllComparators();
    void generateReports();
};

#endif // RUNALLCOMPARATORSINTERACTOR_H
