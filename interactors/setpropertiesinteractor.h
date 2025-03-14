#ifndef SETPROPERTIESINTERACTOR_H
#define SETPROPERTIESINTERACTOR_H

#include <interfaces/imageprocessor.h>
#include <interfaces/mainwindowcallbacks.h>


class SetPropertiesInteractor
{
public:
    SetPropertiesInteractor(shared_ptr<IImageProcessor> processor, IMainWindowCallbacks *callback);

    void allowUserToSetPropertiesIfNeed();

private:
    shared_ptr<IImageProcessor> processor;
    IMainWindowCallbacks *callback;        // long live object
};

#endif // SETPROPERTIESINTERACTOR_H
