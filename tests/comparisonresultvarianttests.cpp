#include "comparisonresultvarianttests.h"

#include <QtTest>
#include <imageprocessorsmanager/comparisonresultvariant.h>

void ComparisonResultVariantTests::testDefaultConstructor() {
    ComparisonResultVariant variant;

    // Verify that the type is None
    QCOMPARE(variant.type(), ComparisonResultVariantType::None);
}

void ComparisonResultVariantTests::testImageConstructor() {
    QImage image(100, 100, QImage::Format_RGB32); // Create a dummy image
    ComparisonResultVariant variant(image);

    // Verify that the type is Image
    QCOMPARE(variant.type(), ComparisonResultVariantType::Image);

    // Verify that the image can be retrieved correctly
    QCOMPARE(variant.imageResult(), image);
}

void ComparisonResultVariantTests::testStringConstructor() {
    QString text = "Test String";
    ComparisonResultVariant variant(text);

    // Verify that the type is String
    QCOMPARE(variant.type(), ComparisonResultVariantType::String);

    // Verify that the string can be retrieved correctly
    QCOMPARE(variant.stringResult(), text);
}

void ComparisonResultVariantTests::testImageResult() {
    QImage image(50, 50, QImage::Format_RGB32); // Create a dummy image
    ComparisonResultVariant variant(image);

    // Verify that the image can be retrieved correctly
    QCOMPARE(variant.imageResult(), image);

    // Attempt to call imageResult on a non-image variant and expect an exception
    ComparisonResultVariant stringVariant(QString("Test"));
    QVERIFY_EXCEPTION_THROWN(stringVariant.imageResult(), std::runtime_error);
}

void ComparisonResultVariantTests::testStringResult() {
    QString text = "Another Test String";
    ComparisonResultVariant variant(text);

    // Verify that the string can be retrieved correctly
    QCOMPARE(variant.stringResult(), text);

    // Attempt to call stringResult on a non-string variant and expect an exception
    QImage image(30, 30, QImage::Format_RGB32);
    ComparisonResultVariant imageVariant(image);
    QVERIFY_EXCEPTION_THROWN(imageVariant.stringResult(), std::runtime_error);
}

// Test the type method
void ComparisonResultVariantTests::testType() {
    // Default constructor should result in None type
    ComparisonResultVariant defaultVariant;
    QCOMPARE(defaultVariant.type(), ComparisonResultVariantType::None);

    // Image constructor should result in Image type
    QImage image(20, 20, QImage::Format_RGB32);
    ComparisonResultVariant imageVariant(image);
    QCOMPARE(imageVariant.type(), ComparisonResultVariantType::Image);

    // String constructor should result in String type
    QString text = "Sample Text";
    ComparisonResultVariant stringVariant(text);
    QCOMPARE(stringVariant.type(), ComparisonResultVariantType::String);
}

