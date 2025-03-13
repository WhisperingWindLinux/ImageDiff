#ifndef SETPROPERTIESINTERACTOR_H
#define SETPROPERTIESINTERACTOR_H

#include <interfaces/imageprocessor.h>
#include <interfaces/mainwindowcallbacks.h>


class SetPropertiesInteractor
{
public:
    SetPropertiesInteractor(AImageProcessor *processor, AMainWindowCallbacks *callback);

    void allowUserToSetPropertiesIfNeed();

private:
    AImageProcessor *processor;
    AMainWindowCallbacks *callback;
};

#endif // SETPROPERTIESINTERACTOR_H
