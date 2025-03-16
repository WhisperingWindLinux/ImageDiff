#include "setpropertiesinteractortests.h"

#include <qtest.h>

#include <tests/mocks/mockImageprocessor.h>
#include <tests/mocks/mockmainwindowcallbacks.h>
#include <business/imageanalysis/setprocessorpropertiesinteractor.h>

void SetPropertiesInteractorTests::testAllowUserToSetPropertiesIfNeed_NoDefaultProperties() {
    // Arrange
    auto processor = make_shared<MockImageProcessor>();
    MockMainWindowCallbacks callbacks;

    processor->defaultProperties = {}; // No default properties
    SetProcessorPropertiesInteractor interactor(processor, &callbacks);

    // Act
    interactor.allowUserToSetPropertiesIfNeed();

    // Assert
    QCOMPARE(processor->updatedProperties.size(), 0);
}

void SetPropertiesInteractorTests::testAllowUserToSetPropertiesIfNeed_UserUpdatesProperties() {
    // Arrange
    auto processor = make_shared<MockImageProcessor>();
    MockMainWindowCallbacks callbacks;

    // Default properties
    Property property = Property::createIntProperty("TestProperty", "Description", 5);
    processor->defaultProperties = { property };

    // User updates properties
    Property updatedProperty = Property::createIntProperty("TestProperty", "Description", 10);
    callbacks.updatedPropertiesFromUser = { updatedProperty };

    SetProcessorPropertiesInteractor interactor(processor, &callbacks);

    // Act
    interactor.allowUserToSetPropertiesIfNeed();

    // Assert
    QCOMPARE(processor->updatedProperties.size(), 1);
    QCOMPARE(processor->updatedProperties[0].toString(),
             Property::createIntProperty("TestProperty", "Description", 10).toString());
}

void SetPropertiesInteractorTests::testAllowUserToSetPropertiesIfNeed_UserDoesNotUpdateProperties() {
    // Arrange
    auto processor = make_shared<MockImageProcessor>();
    MockMainWindowCallbacks callbacks;

    // Default properties
    Property property = Property::createIntProperty("TestProperty", "Description", 5);
    processor->defaultProperties = { property };

    // User does not update properties (empty list returned)
    callbacks.updatedPropertiesFromUser = {};

    SetProcessorPropertiesInteractor interactor(processor, &callbacks);

    // Act
    interactor.allowUserToSetPropertiesIfNeed();

    // Assert
    QCOMPARE(processor->updatedProperties.size(), 0);
}
