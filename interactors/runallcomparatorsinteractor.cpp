#include "runallcomparatorsinteractor.h"
#include "interfaces/iprogressdialog.h"

#include <QPdfWriter>
#include <QtGui/qtextcursor.h>
#include <qdir.h>
#include <qtextdocument.h>
#include <QtCore/qdebug.h>
#include <QtGui/qpainter.h>
#include <imageprocessorsmanager/imageprocessorsmanager.h>
#include <interactors/formatters/htmlreportformatter.h>
#include <QDesktopServices>

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
    executeAllComparators();
    generateReports();
}

void RunAllComparatorsInteractor::executeAllComparators() {
    comparatorsResults.clear();
    comporatorsNames.clear();
    descriptions.clear();
    auto manager = ImageProcessorsManager::instance();
    auto comparators = manager->getAllComparators();
    if (comparators.size() == 0) {
        return;
    }
    progressDialog->showProgressDialog("Run All Comparators", comparators.size());
    int runCounter = 0;
    foreach(auto comparator, comparators) {
        try {
            auto result = comparator->compare(firstImage, secondImage);
            if (result != nullptr) {
                comporatorsNames.append(comparator->getFullName());
                comparatorsResults.append(result);
                descriptions.append(comparator->getDescription());
            }
        } catch(runtime_error &e) {
            qDebug() << e.what();
        }
        runCounter++;
        progressDialog->onUpdateProgressValue(runCounter);
    }
}

void RunAllComparatorsInteractor::generateReports() {
    if (comparatorsResults.size() != comporatorsNames.size()) {
        return;
    }
    QList<QPair<QString, QImage>> comparatorsCombinedResults;
    for (int i = 0; i < comparatorsResults.size(); i++) {
        auto result = comparatorsResults[i];
        auto name = comporatorsNames[i];
        if (result->type() == ComparisonResultVariantType::String) {
             comparatorsCombinedResults.append({result->stringResult(), QImage()});
        }
        else if (result->type() == ComparisonResultVariantType::Image){
            comparatorsCombinedResults.append({name, result->imageResult()});
        }
        else {
            continue;
        }
    }

    bool isOk = HtmlReportFormatter::createReportPage(reportDirPath,
                                                      firstImage.getImage(),
                                                      secondImage.getImage(),
                                                      firstImage.getName(),
                                                      secondImage.getName(),
                                                      comparatorsCombinedResults,
                                                      descriptions);

    if (isOk) {
        progressDialog->onMessage("The report saved to " + reportDirPath + ".");
        QDesktopServices::openUrl("file://" + reportDirPath + QDir::separator() + "report.html");
    } else {
        progressDialog->onError("Error: Unable to generate the report.");
    }
}


