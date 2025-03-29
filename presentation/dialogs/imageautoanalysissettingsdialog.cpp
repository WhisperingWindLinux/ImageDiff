#include "imageautoanalysissettingsdialog.h"
#include "business/imageanalysis/autoanalysissettingsinteractor.h"

#include <domain/interfaces/business/icomparator.h>

ImageAutoAnalysisSettingsDialog::ImageAutoAnalysisSettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Auto-Analysis Settings");

    m_interactor = new AutoAnalysisSettingsInteractor();

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Add description label
    QLabel* descriptionLabel = new QLabel("You can add or remove image processing "
                                          "algorithms from the auto analysis toolbox", this);
    mainLayout->addWidget(descriptionLabel);

    // Create layout for checkboxes
    m_checkBoxLayout = new QVBoxLayout();
    mainLayout->addLayout(m_checkBoxLayout);

    // Dynamically create checkboxes from the interactor's comparators
    auto comparators = m_interactor->getComparators();
    foreach (auto comparator, comparators) {
        QCheckBox* checkBox = new QCheckBox(comparator->getFullName(), this);
        checkBox->setChecked(comparator->isEnabled());
        m_checkBoxes.append(checkBox);
        m_checkBoxLayout->addWidget(checkBox);
    }

    // Add Save and Cancel buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* saveButton = new QPushButton("Save", this);
    QPushButton* cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Connect button signals to slots
    connect(saveButton, &QPushButton::clicked, this, &ImageAutoAnalysisSettingsDialog::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ImageAutoAnalysisSettingsDialog::onCancelClicked);
}

void ImageAutoAnalysisSettingsDialog::onSaveClicked() {
    // Save the state of each checkbox to the associated comparator
    auto comparators = m_interactor->getComparators();
    for (int i = 0; i < m_checkBoxes.size(); ++i) {
        bool isEnabled = m_checkBoxes[i]->isChecked();
        comparators[i]->setEnabled(isEnabled);
        m_interactor->saveComparatorState(comparators[i]->getShortName(), isEnabled);
    }
    accept(); // Close the dialog
}

void ImageAutoAnalysisSettingsDialog::onCancelClicked() {
    reject(); // Close the dialog without saving
}
