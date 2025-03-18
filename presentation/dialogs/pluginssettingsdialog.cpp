#include "pluginssettingsdialog.h"


PluginsSettingsDialog::PluginsSettingsDialog(QWidget *parent)
    : QDialog(parent) {

    interactor = make_unique<PluginSettingsInteractor>();

    setWindowTitle("Plugins Settings");

    QLabel* pythonLabel = new QLabel("Python interpreter:");
    pythonInterpreterEdit = new QTextEdit();
    QLabel* pluginsDirLabel = new QLabel("Plugins directory:");
    pluginsDirectoryEdit = new QTextEdit();
    QPushButton* okButton = new QPushButton("OK");

    loadCurrentSettings();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(pythonLabel);
    mainLayout->addWidget(pythonInterpreterEdit);
    mainLayout->addWidget(pluginsDirLabel);
    mainLayout->addWidget(pluginsDirectoryEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &PluginsSettingsDialog::onOkClicked);
}

void PluginsSettingsDialog::onOkClicked() {
    auto pythonInterpreterPath = pythonInterpreterEdit->toPlainText();
    auto pluginsDirectoryPath = pluginsDirectoryEdit->toPlainText();

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
