#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QObject>

class QTextEdit;

class HelpDialog : public QDialog
{
    Q_OBJECT
public:
    HelpDialog(QString &helpMessage);

private:
    QTextEdit *textEdit;
};

#endif // HELPDIALOG_H
