#ifndef PROPERTYTESTS_H
#define PROPERTYTESTS_H

#include <QObject>

class PropertyTests : public QObject
{
    Q_OBJECT
private slots:
    // Tests for constructors
    void testRealPropertyConstructor();
    void testIntegerPropertyConstructor();

    // Tests for static methods
    void testCreateIntPropertyWithoutConstraints();
    void testCreateIntPropertyWithConstraints();
    void testCreateRealPropertyWithoutConstraints();
    void testCreateRealPropertyWithConstraints();
    void testCreateStringProperty();

    // Edge cases
    void testEmptyAlternativesForStringProperty();
    void testInvalidDefaultIndexForStringProperty();
};

#endif // PROPERTYTESTS_H
