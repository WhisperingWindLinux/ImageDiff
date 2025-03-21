#include "runallcomparatorsinteractor.h"

#include <qdir.h>
#include <qtextdocument.h>
#include <QtCore/qdebug.h>
#include <QtGui/qpainter.h>
#include <QDesktopServices>
#include <presentation/presenters/htmlreportpresenter.h>
#include <domain/valueobjects/autocomparisonreportentry.h>
#include <domain/interfaces/iprogressdialog.h>
#include <business/utils/imagesinfo.h>
#include "imageprocessorsmanager.h"

RunAllComparatorsInteractor::RunAllComparatorsInteractor(IProgressDialog *callback,
                                                         const ComparableImage &firstImage,
                                                         const ComparableImage &secondImage,
                                                         const QString &reportDirPath
                                                         )
    : callback(callback),
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
    callback->showProgressDialog("Run All Comparators", comparators.size());
    int runCounter = 0;
    foreach(auto comparator, comparators) {
        if (!comparator->isPartOfAutoReportingToolbox()) {
            continue;
        }
        if (callback->wasCanceled()) {
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
        callback->onUpdateProgressValue(runCounter);
    }
    callback->onUpdateProgressValue(INT32_MAX);
    return entries;
}

void RunAllComparatorsInteractor::generateReports(QList<AutocomparisonReportEntry> &entries) {

    bool isOk = HtmlReportPresenter::createExtendedReportPage(reportDirPath,
                                                              firstImage,
                                                              secondImage,
                                                              entries
                                                              );

    if (isOk) {
        callback->onMessage("The report saved to " + reportDirPath + ".");
        QDesktopServices::openUrl("file://" + reportDirPath + QDir::separator() + "report.html");
    } else {
        callback->onError("Error: Unable to generate the report.");
    }
}
