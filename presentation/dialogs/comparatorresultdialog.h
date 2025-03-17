#ifndef COMPARATORRESULTDIALOG_H
#define COMPARATORRESULTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDir>

class ComparatorResultDialog : public QDialog {
    Q_OBJECT

public:
    ComparatorResultDialog(QString &message,
                           QString &comparatorFullName,
                           QString &comparatorDescription,
                           QString &firstImageFilePath,
                           QString &secondImageFilePath,
                           QWidget *parent = nullptr
                           );

private:
    QString message;
    QString firstFilePath;
    QString secondFilePath;
    QString comparatorFullName;
    QString comparatorDescription;

    QLabel *label;
    QPushButton *closeButton;
    QPushButton *saveButton;

    void setupUI();

private slots:
    void onSaveClicked();
};

#endif // COMPARATORRESULTDIALOG_H
