#include "propertytests.h"
#include <QTest>
#include <cfloat>
#include <domain/valueobjects/property.h>

void PropertyTests::testRealPropertyConstructor() {
    Property realProperty("Length", "The length of an object", 10.5, 0.0, 100.0);

    QCOMPARE(realProperty.propertyName, QString("Length"));
    QCOMPARE(realProperty.propertyDescription, QString("The length of an object"));
    QCOMPARE(realProperty.doubleValue, 10.5);
    QCOMPARE(realProperty.min, 0.0);
    QCOMPARE(realProperty.max, 100.0);
}

void PropertyTests::testIntegerPropertyConstructor() {
    Property intProperty("Count", "The number of items", 5, 0, 10);

    QCOMPARE(intProperty.propertyName, QString("Count"));
    QCOMPARE(intProperty.propertyDescription, QString("The number of items"));
    QCOMPARE(intProperty.doubleValue, 5.0); // Stored as double
    QCOMPARE(intProperty.min, 0.0);          // Stored as double
    QCOMPARE(intProperty.max, 10.0);         // Stored as double
}

void PropertyTests::testCreateIntPropertyWithoutConstraints() {
    Property intProp = Property::createIntProperty("Age", "The age of a person", 25);

    QCOMPARE(intProp.propertyName, QString("Age"));
    QCOMPARE(intProp.propertyDescription, QString("The age of a person"));
    QCOMPARE(intProp.doubleValue, 25.0); // Stored as double
    QCOMPARE(intProp.min, static_cast<double>(INT_MIN));
    QCOMPARE(intProp.max, static_cast<double>(INT_MAX));
}

void PropertyTests::testCreateIntPropertyWithConstraints() {
    Property intProp = Property::createIntProperty("Score", "Game score", 50, 0, 100);

    QCOMPARE(intProp.propertyName, QString("Score"));
    QCOMPARE(intProp.propertyDescription, QString("Game score"));
    QCOMPARE(intProp.doubleValue, 50.0); // Stored as double
    QCOMPARE(intProp.min, 0.0);
    QCOMPARE(intProp.max, 100.0);
}

void PropertyTests::testCreateRealPropertyWithoutConstraints() {
    Property realProp = Property::createRealProperty("Temperature", "Room temperature", 22);

    QCOMPARE(realProp.propertyName, QString("Temperature"));
    QCOMPARE(realProp.propertyDescription, QString("Room temperature"));
    QCOMPARE(realProp.doubleValue, 22.0); // Stored as double
    QCOMPARE(realProp.min, -DBL_MAX);
    QCOMPARE(realProp.max, DBL_MAX);
}

void PropertyTests::testCreateRealPropertyWithConstraints() {
    Property realProp = Property::createRealProperty("Weight", "The weight of an object", 75.5, 0.0, 200.0);

    QCOMPARE(realProp.propertyName, QString("Weight"));
    QCOMPARE(realProp.propertyDescription, QString("The weight of an object"));
    QCOMPARE(realProp.doubleValue, 75.5);
    QCOMPARE(realProp.min, 0.0);
    QCOMPARE(realProp.max, 200.0);
}

void PropertyTests::testCreateStringProperty() {
    QStringList alternatives = {"Option1", "Option2", "Option3"};
    Property stringProp = Property::createAlternativesProperty("Choice", "User choice", alternatives, 1);

    QCOMPARE(stringProp.propertyName, QString("Choice"));
    QCOMPARE(stringProp.propertyDescription, QString("User choice"));
    QCOMPARE(stringProp.alternativesValue.size(), 3);
    QCOMPARE(stringProp.alternativesValue[1], QString("Option2"));
    QCOMPARE(stringProp.doubleValue, 1.0); // Index stored as double
}

void PropertyTests::testEmptyAlternativesForStringProperty() {
    QStringList emptyAlternatives;

    QVERIFY_EXCEPTION_THROWN(
        Property::createAlternativesProperty("Choice", "User choice", emptyAlternatives, 0),
        std::invalid_argument
        );
}

void PropertyTests::testInvalidDefaultIndexForStringProperty() {
    QStringList alternatives = {"Option1", "Option2"};

    QVERIFY_EXCEPTION_THROWN(
        Property::createAlternativesProperty("Choice", "User choice", alternatives, -1),
        std::out_of_range
        );

    QVERIFY_EXCEPTION_THROWN(
        Property::createAlternativesProperty("Choice", "User choice", alternatives, 2),
        std::out_of_range
        );
}
