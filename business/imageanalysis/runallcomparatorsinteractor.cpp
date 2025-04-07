#include "runallcomparatorsinteractor.h"

#include <qdir.h>
#include <qtextdocument.h>
#include <QtCore/qdebug.h>
#include <QtGui/qpainter.h>
#include <QDesktopServices>
#include <domain/valueobjects/autocomparisonreportentry.h>
#include <domain/interfaces/presentation/iprogressdialog.h>
#include <business/utils/imagesinfo.h>
#include <business/imageanalysis/comporators/formatters/htmlreportpresenter.h>
#include "imageprocessorsmanager.h"

RunAllComparatorsInteractor::RunAllComparatorsInteractor(IProgressDialog *callback,
                                                         const ComparableImage &firstImage,
                                                         const ComparableImage &secondImage,
                                                         const QString &reportDirPath
                                                         )
    : mCallback(callback),
    mFirstImage(firstImage),
    mSecondImage(secondImage),
    mReportDirPath(reportDirPath)
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
    mCallback->showProgressDialog("Run All Comparators", comparators.size());
    int runCounter = 0;
    foreach(auto comparator, comparators) {
        if (!comparator->isPartOfAutoReportingToolbox()) {
            continue;
        }
        if (!comparator->isEnabled()) {
            continue;
        }
        if (mCallback->wasCanceled()) {
            return {};
        }
        try {
            comparator->reset();
            auto result = comparator->compare(mFirstImage, mSecondImage);
            if (result != nullptr) {
                auto proicessorInfo = manager->getProcessorInfoByProcessorShortName(comparator->getShortName());
                entries.append({ result, proicessorInfo });
            }
        } catch(runtime_error &e) {
            qDebug() << e.what();
        }
        runCounter++;
        mCallback->onUpdateProgressValue(runCounter);
    }
    mCallback->onUpdateProgressValue(INT32_MAX);
    return entries;
}

void RunAllComparatorsInteractor::generateReports(QList<AutocomparisonReportEntry> &entries) {

    bool isOk = HtmlReportPresenter::createExtendedReportPage(mReportDirPath,
                                                              mFirstImage,
                                                              mSecondImage,
                                                              entries
                                                              );

    if (isOk) {
        mCallback->onMessage("The report saved to " + mReportDirPath + ".");
        QDesktopServices::openUrl("file://" + mReportDirPath + QDir::separator() + "report.html");
    } else {
        mCallback->onError("Unable to generate the report.");
    }
}
