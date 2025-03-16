#include "setprocessorpropertiesinteractor.h"

SetProcessorPropertiesInteractor::SetProcessorPropertiesInteractor(shared_ptr<IImageProcessor> processor,
                                                 IMainWindowCallbacks *callback
                                                 )
    : processor(processor),
      callback(callback)
{

}

void SetProcessorPropertiesInteractor::allowUserToSetPropertiesIfNeed() {
    auto properties = processor->getDefaultProperties();
    if (properties.empty()) {
        return;
    }
    auto newProperties = callback->getUpdatedPropertiesFromUser(processor->getShortName(),
                                                                processor->getDescription(),
                                                                properties
                                                                );
    if (!newProperties.empty()) {
        processor->setProperties(newProperties);
    }
}
