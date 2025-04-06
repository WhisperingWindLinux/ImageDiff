#ifndef IOTHERAPPINSTANCESINTERACTORCALLBACK_H
#define IOTHERAPPINSTANCESINTERACTORCALLBACK_H

#include <QtCore/qobject.h>

class OtherAppInstancesInteractorCallback {
public:
    virtual void onOtherAppInstanceOpened() = 0;
    virtual void showError(const QString &errorMessage) = 0;
};

#endif // IOTHERAPPINSTANCESINTERACTORCALLBACK_H
