#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QObject>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qtextbrowser.h>

#include <domain/valueobjects/imageprocessorsinfo.h>

#include <presentation/dialogs/formatters/helphtmlformatter.h>

class QTextEdit;

class HelpDialog : public QDialog
{
public:
    explicit HelpDialog(const QList<ImageProcessorInfo> &algorithms, QWidget *parent = nullptr);

private slots:
    void onAlgorithmSelected(int index);

private:
    QList<ImageProcessorInfo> algorithms;
    QListWidget *listWidget;
    QTextBrowser *infoBrowser;
    QTextBrowser *imageAreaSelectionBrowser; // For the second tab

};

#endif // HELPDIALOG_H
