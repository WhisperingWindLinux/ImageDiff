#include "tst_imagevalidationrules.h"

#include <business/validation/imagevalidationrules.h>

// Test: both images are empty
void TestImageValidationRules::testBothImagesAreNull() {
    QPixmap emptyPixmap1;
    QPixmap emptyPixmap2;

    ImagesPtr images = std::make_shared<Images>(emptyPixmap1, emptyPixmap2, "", "");
    ImageValidationRules validator(images);

    auto result = validator.isValid();
    QVERIFY(result.has_value());
    QCOMPARE(result, "One or both images are empty.");
}

// Test: images have different sizes
void TestImageValidationRules::testDifferentSizes() {
    QPixmap pixmap1(100, 100);
    pixmap1.fill(Qt::red);

    QPixmap pixmap2(200, 200);
    pixmap2.fill(Qt::red);

    ImagesPtr images = std::make_shared<Images>(pixmap1, pixmap2, "", "");
    ImageValidationRules validator(images);

    auto result = validator.isValid();
    QVERIFY(result.has_value());
    QCOMPARE(result, "The images do not have the same dimensions.");
}

// Test: valid images
void TestImageValidationRules::testValidImages() {
    QPixmap pixmap1(100, 100);
    pixmap1.fill(Qt::red);

    QPixmap pixmap2(100, 100);
    pixmap2.fill(Qt::red);

    ImagesPtr images = std::make_shared<Images>(pixmap1, pixmap2, "", "");
    ImageValidationRules validator(images);

    auto result = validator.isValid();
    QVERIFY(!result.has_value()); // Result should be empty (valid images)
}

