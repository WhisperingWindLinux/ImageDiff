#include "runallcomparatorsinteractor.h"

#include <domain/interfaces/iprogressdialog.h>
#include "imageprocessorsmanager.h"

#include <qdir.h>
#include <qtextdocument.h>
#include <QtCore/qdebug.h>
#include <QtGui/qpainter.h>
#include <QDesktopServices>
#include <presentation/presenters/htmlreportpresenter.h>
#include <domain/valueobjects/autocomparisonreportentry.h>

RunAllComparatorsInteractor::RunAllComparatorsInteractor(IProgressDialog *progressDialog,
                                                         ComparableImage firstImage,
                                                         ComparableImage secondImage,
                                                         QString reportDirPath
                                                         )
    : progressDialog(progressDialog),
    firstImage(firstImage),
    secondImage(secondImage),
    reportDirPath(reportDirPath)
{

}

void RunAllComparatorsInteractor::run() {
    auto entries = executeAllComparators();
    if (entries.size() != 0) {
        generateReports(entries);
    }
}

QList<AutocomparisonReportEntry> RunAllComparatorsInteractor::executeAllComparators() {
    QList<AutocomparisonReportEntry> entries;
    auto manager = ImageProcessorsManager::instance();
    auto comparators = manager->getAllComparators();
    if (comparators.size() == 0) {
        return {};
    }
    progressDialog->showProgressDialog("Run All Comparators", comparators.size());
    int runCounter = 0;
    foreach(auto comparator, comparators) {
        if (!comparator->isPartOfAutoReportingToolbox()) {
            continue;
        }
        if (progressDialog->wasCanceled()) {
            return {};
        }
        try {
            auto result = comparator->compare(firstImage, secondImage);
            if (result != nullptr) {
                entries.append({result, comparator->getFullName(), comparator->getDescription()});
            }
        } catch(runtime_error &e) {
            qDebug() << e.what();
        }
        runCounter++;
        progressDialog->onUpdateProgressValue(runCounter);
    }
    progressDialog->onUpdateProgressValue(INT32_MAX);
    return entries;
}

void RunAllComparatorsInteractor::generateReports(QList<AutocomparisonReportEntry> entries) {

    bool isOk = HtmlReportPresenter::createExtendedReportPage(reportDirPath,
                                                              firstImage,
                                                              secondImage,
                                                              entries
                                                              );

    if (isOk) {
        progressDialog->onMessage("The report saved to " + reportDirPath + ".");
        QDesktopServices::openUrl("file://" + reportDirPath + QDir::separator() + "report.html");
    } else {
        progressDialog->onError("Error: Unable to generate the report.");
    }
}


