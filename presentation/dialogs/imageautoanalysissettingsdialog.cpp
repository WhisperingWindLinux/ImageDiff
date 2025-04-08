#include "imageautoanalysissettingsdialog.h"
#include "business/imageanalysis/autoanalysissettingsinteractor.h"

#include <domain/interfaces/business/icomparator.h>

ImageAutoAnalysisSettingsDialog::ImageAutoAnalysisSettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Auto-Analysis Settings");

    mInteractor = new AutoAnalysisSettingsInteractor();

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Add description label
    QLabel* descriptionLabel = new QLabel("You can add or remove image processing "
                                          "algorithms from the auto analysis toolbox", this);
    mainLayout->addWidget(descriptionLabel);

    // Create layout for checkboxes
    mCheckBoxLayout = new QVBoxLayout();
    mainLayout->addLayout(mCheckBoxLayout);

    // Dynamically create checkboxes from the interactor's comparators
    auto comparators = mInteractor->getComparators();
    foreach (auto comparator, comparators) {
        QCheckBox* checkBox = new QCheckBox(comparator->getFullName(), this);
        checkBox->setChecked(comparator->isEnabled());
        mCheckBoxes.append(checkBox);
        mCheckBoxLayout->addWidget(checkBox);
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
    auto comparators = mInteractor->getComparators();
    for (int i = 0; i < mCheckBoxes.size(); ++i) {
        bool isEnabled = mCheckBoxes[i]->isChecked();
        comparators[i]->setEnabled(isEnabled);
        mInteractor->saveComparatorState(comparators[i]->getShortName(), isEnabled);
    }
    accept(); // Close the dialog
}

void ImageAutoAnalysisSettingsDialog::onCancelClicked() {
    reject(); // Close the dialog without saving
}
