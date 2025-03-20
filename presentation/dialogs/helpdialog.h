#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QObject>

class QTextEdit;

class HelpDialog : public QDialog
{
    Q_OBJECT
public:
    HelpDialog(const QString &helpMessage);

private:
    QTextEdit *textEdit;
};

#endif // HELPDIALOG_H
