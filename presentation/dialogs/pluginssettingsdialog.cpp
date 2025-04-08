#include "pluginssettingsdialog.h"

#include <qlineedit.h>


PluginsSettingsDialog::PluginsSettingsDialog(QWidget *parent)
    : QDialog(parent) {

    mInteractor = make_unique<PluginsSettingsInteractor>();

    setWindowTitle("Plugins Settings");

    QLabel *aboutLabel = new QLabel("The application supports plugins written in Python. "
                                    "You can find examples of plugins in the developer's"
                                    " GitHub repository https://github.com/WhisperingWindLinux.");
    aboutLabel->setWordWrap(true);

    QLabel* pythonLabel = new QLabel("Python interpreter:");
    mPythonInterpreterEdit = new QLineEdit();
    QLabel* pluginsDirLabel = new QLabel("Plugins directory:");
    mPluginsDirectoryEdit = new QLineEdit();

    QPushButton* okButton = new QPushButton("OK");
    QPushButton* cancelButton = new QPushButton("Cancel");

    loadCurrentSettings();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(aboutLabel);
    mainLayout->addSpacing(1);
    mainLayout->addWidget(pythonLabel);
    mainLayout->addWidget(mPythonInterpreterEdit);
    mainLayout->addSpacing(1);
    mainLayout->addWidget(pluginsDirLabel);
    mainLayout->addWidget(mPluginsDirectoryEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &PluginsSettingsDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &PluginsSettingsDialog::close);
}

void PluginsSettingsDialog::onOkClicked() {
    auto pythonInterpreterPath = mPythonInterpreterEdit->text();
    auto pluginsDirectoryPath = mPluginsDirectoryEdit->text();

    QString error;
    PluginsSettings pluginsSettings = { pythonInterpreterPath, pluginsDirectoryPath };
    bool isOk = mInteractor->updatePluginSettings(pluginsSettings, error);
    if (!isOk) {
        showError(error);
    }
    accept();
}

void PluginsSettingsDialog::loadCurrentSettings() {
    PluginsSettings settings = mInteractor->getPluginSettings();
    mPythonInterpreterEdit->setText(settings.pythonInterpreterPath);
    mPluginsDirectoryEdit->setText(settings.pluginsDirectoryPath);
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
