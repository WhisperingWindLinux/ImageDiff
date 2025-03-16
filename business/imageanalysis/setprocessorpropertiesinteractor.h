#ifndef SETPROCESSORPROPERTIESINTERACTOR_H
#define SETPROCESSORPROPERTIESINTERACTOR_H

#include <domain/interfaces/imageprocessor.h>
#include <domain/interfaces/mainwindowcallbacks.h>


class SetProcessorPropertiesInteractor
{
public:
    SetProcessorPropertiesInteractor(shared_ptr<IImageProcessor> processor, IMainWindowCallbacks *callback);

    void allowUserToSetPropertiesIfNeed();

private:
    shared_ptr<IImageProcessor> processor;
    IMainWindowCallbacks *callback;
};

#endif // SETPROCESSORPROPERTIESINTERACTOR_H
