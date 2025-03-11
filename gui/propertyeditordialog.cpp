#include "propertyeditordialog.h"

PropertyEditorDialog::PropertyEditorDialog(QList<Property> properties, QWidget *parent)
    : QDialog(parent), deafultProperties(properties) {

    setWindowTitle("Edit Properties");

    mainLayout = new QVBoxLayout(this);

    // Dynamically create controls for each property
    for (auto it = this->deafultProperties.begin(); it != this->deafultProperties.end(); ++it) {

        auto property = (*it);

        // Create a horizontal layout for each property
        QHBoxLayout *propertyLayout = new QHBoxLayout;

        // QLabel for property name
        QLabel *label = new QLabel(property.getPoropertyName(), this);
        label->setToolTip(property.getPropertyDescription()); // Set tooltip with description
        propertyLayout->addWidget(label);

        // Create the appropriate editor widget based on the property type
        QWidget *editor = nullptr;

        switch (property.getPropertyType()) {
        case Property::Type::Integer: {
            QSpinBox *spinBox = new QSpinBox(this);
            spinBox->setMinimum(static_cast<int>(property.getMinValue()));
            spinBox->setMaximum(static_cast<int>(property.getMaxValue()));
            spinBox->setValue(static_cast<int>(property.getValue()));
            editor = spinBox;
            break;
        }
        case Property::Type::Real: {
            QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox(this);
            doubleSpinBox->setMinimum(property.getMinValue());
            doubleSpinBox->setMaximum(property.getMaxValue());
            doubleSpinBox->setValue(property.getValue());
            doubleSpinBox->setDecimals(2); // Set precision for floating-point numbers
            editor = doubleSpinBox;
            break;
        }
        case Property::Type::Alternatives: {
            QComboBox *comboBox = new QComboBox(this);
            comboBox->addItems(property.getAlternatives());
            comboBox->setCurrentIndex(static_cast<int>(property.getValue()));
            editor = comboBox;
            break;
        }
        }

        if (editor) {
            propertyLayout->addWidget(editor);
            editors.append(editor); // Store the editor for later use
        }

        mainLayout->addLayout(propertyLayout);
    }

    // Add Run and Cancel buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *runButton = new QPushButton("Run", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    buttonLayout->addStretch();
    buttonLayout->addWidget(runButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    // Connect signals to slots
    connect(runButton, &QPushButton::clicked, this, &PropertyEditorDialog::onRun);
    connect(cancelButton, &QPushButton::clicked, this, &PropertyEditorDialog::reject);
}

void PropertyEditorDialog::onRun() {
    // Update property values from the editors
    for (int i = 0; i < deafultProperties.size(); ++i) {
        auto property = deafultProperties[i];
        QWidget *editor = editors[i];

        switch (property.getPropertyType()) {
        case Property::Type::Integer: {
            QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor);
            if (spinBox) {
                Property prop = Property::createIntProperty(
                    property.getPoropertyName(),
                    property.getPropertyDescription(),
                    spinBox->value(),
                    static_cast<int>(property.getMinValue()),
                    static_cast<int>(property.getMaxValue())
                    );
                updatedProperties.append(prop);
            }
            break;
        }
        case Property::Type::Real: {
            QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);
            if (doubleSpinBox) {
                Property prop = Property::createRealProperty(
                    property.getPoropertyName(),
                    property.getPropertyDescription(),
                    doubleSpinBox->value(),
                    property.getMinValue(),
                    property.getMaxValue()
                    );
                updatedProperties.append(prop);
            }
            break;
        }
        case Property::Type::Alternatives: {
            QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
            if (comboBox) {
                Property prop = Property::createStringProperty(
                    property.getPoropertyName(),
                    property.getPropertyDescription(),
                    property.getAlternatives(),
                    comboBox->currentIndex()
                    );
                updatedProperties.append(prop);
            }
            break;
        }
        }
    }

    accept(); // Close dialog with Accepted result
}

QList<Property> PropertyEditorDialog::getUpdatedProperties() const {
    if (updatedProperties.empty()) {
        return deafultProperties;
    } else {
        return updatedProperties;
    }
}
