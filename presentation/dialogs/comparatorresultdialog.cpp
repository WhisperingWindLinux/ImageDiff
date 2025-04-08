#include "comparatorresultdialog.h"
#include <QtWidgets/qmessagebox.h>
#include <business/imageanalysis/comporators/formatters/htmlreportpresenter.h>
#include <data/storage/filedialoghandler.h>


ComparatorResultDialog::ComparatorResultDialog(const QString &message,
                                               const QString &comparatorFullName,
                                               const QString &firstImageFilePath,
                                               const QString &secondImageFilePath,
                                               QWidget *parent
                                               )
    : QDialog(parent),
    mMessage(message),
    mFirstFilePath(firstImageFilePath),
    mSecondFilePath(secondImageFilePath),
    mComparatorFullName(comparatorFullName){
    setupUI();
}

void ComparatorResultDialog::setupUI() {
    mMessageLabel = new QLabel(mMessage, this);
    mCloseButton = new QPushButton("Close", this);
    mSaveButton = new QPushButton("Save", this);
    mMessageLabel->setWordWrap(true);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(mMessageLabel);
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addStretch();
    hlayout->addWidget(mSaveButton);
    hlayout->addSpacing(10);
    hlayout->addWidget(mCloseButton);
    vlayout->addItem(hlayout);

    setLayout(vlayout);

    connect(mCloseButton, &QPushButton::clicked, this, &ComparatorResultDialog::close);
    connect(mSaveButton, &QPushButton::clicked, this, &ComparatorResultDialog::onSaveClicked);

    setWindowTitle("Comparison Report");
}

void ComparatorResultDialog::onSaveClicked() {
    QDir parentDir(QFileInfo(mFirstFilePath).absolutePath());
    QString filePath = parentDir.absolutePath() + QDir::separator() + "report.html";

    FileDialogHandler services;
    auto savedPath = services.getUserSaveReportPath(filePath);

    if (savedPath) {
        bool isOk = HtmlReportPresenter::createSimpleReportPage(savedPath.value(),
                                                                mFirstFilePath,
                                                                mSecondFilePath,
                                                                mMessage);
        if (!isOk) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Unable to save the report!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
}


