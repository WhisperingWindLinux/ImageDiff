#include "setpropertiesinteractor.h"

SetPropertiesInteractor::SetPropertiesInteractor(AImageProcessor *processor,
                                                 AMainWindowCallbacks *callback
                                                 )
    : processor(processor),
      callback(callback)
{

}

void SetPropertiesInteractor::allowUserToSetPropertiesIfNeed() {
    auto properties = processor->getDefaultProperties();
    if (properties.empty()) {
        return;
    }
    auto newProperties = callback->getUpdatedPropertiesFromUser(properties);
    if (!newProperties.empty()) {
        processor->setProperties(newProperties);
    }
}
