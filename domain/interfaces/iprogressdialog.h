#ifndef IPROGRESSDIALOG_H
#define IPROGRESSDIALOG_H

#include <qstring.h>

class IProgressDialog {
public:
    IProgressDialog() = default;
    virtual ~IProgressDialog() = default;

    virtual void showProgressDialog(const QString &caption, int totalSteps) = 0;
    virtual bool wasCanceled() = 0;
    virtual void onUpdateProgressValue(int value) = 0;
    virtual void onMessage(const QString &message) = 0;
    virtual void onError(const QString &error) = 0;
};

#endif // IPROGRESSDIALOG_H
