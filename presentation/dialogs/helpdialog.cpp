#include "helpdialog.h"


HelpDialog::HelpDialog(const QList<ImageProcessorInfo> &algorithms, QWidget *parent)
    : QDialog(parent), algorithms(algorithms) {

    setWindowTitle("Help");
    resize(1000, 600);

    // Создаем виджеты
    QLabel *instructionLabel = new QLabel("Select an algorithm from the list to see its reference information.");
    instructionLabel->setWordWrap(true);

    listWidget = new QListWidget();
    for (const auto &algorithm : algorithms) {
        listWidget->addItem(algorithm.name);
    }

    infoBrowser = new QTextBrowser();
    infoBrowser->setText("Select an algorithm to see details.");

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(listWidget, 1);
    mainLayout->addWidget(infoBrowser, 2);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(instructionLabel);
    layout->addLayout(mainLayout);

    connect(listWidget, &QListWidget::currentRowChanged, this, &HelpDialog::onAlgorithmSelected);
}

void HelpDialog::onAlgorithmSelected(int index) {
    if (index >= 0 && index < algorithms.size()) {
        const ImageProcessorInfo &selectedAlgorithm = algorithms.at(index);
        QString html = HelpHtmlFormatter::formatImageProcessorInfo(selectedAlgorithm);
        infoBrowser->setHtml(html);
    } else {
        infoBrowser->setText("Select an algorithm to see details.");
    }
}
