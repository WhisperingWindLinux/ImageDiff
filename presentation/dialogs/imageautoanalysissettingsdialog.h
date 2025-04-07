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
    AutoAnalysisSettingsInteractor* mInteractor;
    QList<QCheckBox*> mCheckBoxes;             // List of dynamically created checkboxes
    QVBoxLayout* mCheckBoxLayout;              // Layout for checkboxes
};


#endif // IMAGEAUTOANALYSISSETTINGSDIALOG_H
