#ifndef IMAGEAUTOANALYSISSETTINGSDIALOG_H
#define IMAGEAUTOANALYSISSETTINGSDIALOG_H

#include <QDialog>
#include <QList>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class AutoAnalysisSettingsInteractor;

class ImageAutoAnalysisSettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit ImageAutoAnalysisSettingsDialog(QWidget* parent = nullptr);

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    AutoAnalysisSettingsInteractor* m_interactor;
    QList<QCheckBox*> m_checkBoxes;             // List of dynamically created checkboxes
    QVBoxLayout* m_checkBoxLayout;              // Layout for checkboxes
};


#endif // IMAGEAUTOANALYSISSETTINGSDIALOG_H
