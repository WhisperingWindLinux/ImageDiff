#include "comparatorresultdialog.h"
#include <QtWidgets/qmessagebox.h>
#include <presentation/presenters/htmlreportpresenter.h>


ComparatorResultDialog::ComparatorResultDialog(QString &message,
                                               QString &comparatorFullName,
                                               QString &comparatorDescription,
                                               QString &firstImageFilePath,
                                               QString &secondImageFilePath,
                                               QWidget *parent
                                               )
    : QDialog(parent), message(message),
    firstFilePath(firstImageFilePath),
    secondFilePath(secondImageFilePath),
    comparatorFullName(comparatorFullName),
    comparatorDescription(comparatorDescription)
{
    setupUI();
}

void ComparatorResultDialog::setupUI() {
    label = new QLabel(message, this);
    closeButton = new QPushButton("Close", this);
    saveButton = new QPushButton("Save", this);
    label->setWordWrap(true);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(label);
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addStretch();
    hlayout->addWidget(saveButton);
    hlayout->addSpacing(10);
    hlayout->addWidget(closeButton);
    vlayout->addItem(hlayout);

    setLayout(vlayout);

    connect(closeButton, &QPushButton::clicked, this, &ComparatorResultDialog::close);
    connect(saveButton, &QPushButton::clicked, this, &ComparatorResultDialog::onSaveClicked);

    setWindowTitle("Comparison Report");
}

void ComparatorResultDialog::onSaveClicked() {
    QDir parentDir(QFileInfo(firstFilePath).absolutePath());
    QString filePath = parentDir.absolutePath() + QDir::separator() + "report.html";

    QString savePath = QFileDialog::getSaveFileName(this,
                                                    "Save report",
                                                    filePath
                                                    );

    if (!savePath.isEmpty()) {
        bool isOk = HtmlReportPresenter::createSimpleReportPage(savePath,
                                                                firstFilePath,
                                                                secondFilePath,
                                                                comparatorDescription,
                                                                message);
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


