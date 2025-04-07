#ifndef PLUGINSSETTINGSDIALOG_H
#define PLUGINSSETTINGSDIALOG_H

#include <QDialog>
#include <QObject>

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qmessagebox.h>

#include <business/pluginsettingsinteractor.h>

class QLineEdit;

class PluginsSettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit PluginsSettingsDialog(QWidget* parent = nullptr);

private slots:
    void onOkClicked();

private:
    QLineEdit* mPythonInterpreterEdit;
    QLineEdit* mPluginsDirectoryEdit;
    unique_ptr<PluginsSettingsInteractor> mInteractor;

    void loadCurrentSettings();
    void showError(const QString &errorMessage);
};


#endif // PLUGINSSETTINGSDIALOG_H
