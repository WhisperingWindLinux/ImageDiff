#include "comparableimagetests.h"

#include <qimage.h>
#include <qtest.h>
#include <imageprocessorsmanager/comparableimage.h>

void ComparableImageTests::testConstructorWithQImage() {
    QImage image(100, 100, QImage::Format_RGB32);
    QString name = "/path/to/image.jpg";

    ComparableImage comparableImage(image, name);

    // Verify that the image is stored correctly
    QCOMPARE(comparableImage.getImage(), image);

    // Verify that the name is extracted correctly (only the file name, not the full path)
    QCOMPARE(comparableImage.getName(), QString("image.jpg"));
}

void ComparableImageTests::testConstructorWithQPixmap() {
    QPixmap pixmap(100, 100);
    QString name = "/path/to/another_image.png";

    ComparableImage comparableImage(pixmap, name);

    // Verify that the pixmap is converted to QImage and stored correctly
    QCOMPARE(comparableImage.getImage(), pixmap.toImage());

    // Verify that the name is extracted correctly (only the file name, not the full path)
    QCOMPARE(comparableImage.getName(), QString("another_image.png"));
}

void ComparableImageTests::testGetImage() {
    QImage image(50, 50, QImage::Format_ARGB32);
    QString name = "test_image.bmp";

    ComparableImage comparableImage(image, name);

    // Verify that getImage() returns the correct QImage
    QCOMPARE(comparableImage.getImage(), image);
}

void ComparableImageTests::testGetName() {
    QImage image(30, 30, QImage::Format_Grayscale8);
    QString name = "/some/path/to/test_name.jpeg";

    ComparableImage comparableImage(image, name);

    // Verify that getName() returns only the file name
    QCOMPARE(comparableImage.getName(), QString("test_name.jpeg"));
}

