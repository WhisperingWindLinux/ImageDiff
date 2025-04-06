#include "pluginssettingsdialog.h"

#include <qlineedit.h>


PluginsSettingsDialog::PluginsSettingsDialog(QWidget *parent)
    : QDialog(parent) {

    interactor = make_unique<PluginsSettingsInteractor>();

    setWindowTitle("Plugins Settings");

    QLabel *aboutLabel = new QLabel("The application supports plugins written in Python. "
                                    "You can find examples of plugins in the developer's"
                                    " GitHub repository https://github.com/WhisperingWindLinux.");
    aboutLabel->setWordWrap(true);

    QLabel* pythonLabel = new QLabel("Python interpreter:");
    pythonInterpreterEdit = new QLineEdit();
    QLabel* pluginsDirLabel = new QLabel("Plugins directory:");
    pluginsDirectoryEdit = new QLineEdit();

    QPushButton* okButton = new QPushButton("OK");
    QPushButton* cancelButton = new QPushButton("Cancel");

    loadCurrentSettings();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(aboutLabel);
    mainLayout->addSpacing(1);
    mainLayout->addWidget(pythonLabel);
    mainLayout->addWidget(pythonInterpreterEdit);
    mainLayout->addSpacing(1);
    mainLayout->addWidget(pluginsDirLabel);
    mainLayout->addWidget(pluginsDirectoryEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &PluginsSettingsDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &PluginsSettingsDialog::close);
}

void PluginsSettingsDialog::onOkClicked() {
    auto pythonInterpreterPath = pythonInterpreterEdit->text();
    auto pluginsDirectoryPath = pluginsDirectoryEdit->text();

    QString error;
    PluginsSettings pluginsSettings = { pythonInterpreterPath, pluginsDirectoryPath };
    bool isOk = interactor->updatePluginSettings(pluginsSettings, error);
    if (!isOk) {
        showError(error);
    }
    accept();
}

void PluginsSettingsDialog::loadCurrentSettings() {
    PluginsSettings settings = interactor->getPluginSettings();
    pythonInterpreterEdit->setText(settings.pythonInterpreterPath);
    pluginsDirectoryEdit->setText(settings.pluginsDirectoryPath);
}

void PluginsSettingsDialog::showError(const QString &errorMessage) {
    qDebug() << errorMessage;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(errorMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}
