#ifndef COMPARATORRESULTDIALOG_H
#define COMPARATORRESULTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDir>

class ComparatorResultDialog : public QDialog {
    Q_OBJECT

public:
    ComparatorResultDialog(const QString& message,
                           const QString& comparatorFullName,
                           const QString& firstImageFilePath,
                           const QString& secondImageFilePath,
                           QWidget *parent = nullptr
                           );

private:
    QString mMessage;
    QString mFirstFilePath;
    QString mSecondFilePath;
    QString mComparatorFullName;

    QLabel *mMessageLabel;
    QPushButton *mCloseButton;
    QPushButton *mSaveButton;

    void setupUI();

private slots:
    void onSaveClicked();
};

#endif // COMPARATORRESULTDIALOG_H
