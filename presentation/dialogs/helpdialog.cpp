#include "helpdialog.h"

#include <qboxlayout.h>
#include <qtextedit.h>

HelpDialog::HelpDialog(QString &helpMessage) {
    setWindowTitle("Help");

    QVBoxLayout *layout = new QVBoxLayout(this);
    QTextEdit *textEdit = new QTextEdit(this);
    layout->addWidget(textEdit);

    textEdit->setReadOnly(true);
    textEdit->setHtml(helpMessage);

    setMinimumSize(800, 600);
}
