#include <QtGui/qpainter.h>
#include <domain/valueobjects/savefileinfo.h>
#include <qtest.h>

#include "saveimageinfotests.h"

// Test default constructor
void SaveImageInfoTests::testDefaultConstructor() {
    SaveImageInfo saveImageInfo;

    QCOMPARE(saveImageInfo.saveImageInfoType, SaveImageInfoType::None);
    QVERIFY(saveImageInfo.image.isNull()); // QPixmap should be null by default
}

// Test parameterized constructor
void SaveImageInfoTests::testParameterizedConstructor() {
    QPixmap pixmap(100, 100);
    pixmap.fill(Qt::red);

    SaveImageInfo saveImageInfo(SaveImageInfoType::FirstImage, pixmap);

    QCOMPARE(saveImageInfo.saveImageInfoType, SaveImageInfoType::FirstImage);
    QVERIFY(!saveImageInfo.image.isNull());
    QCOMPARE(saveImageInfo.image.size(), QSize(100, 100));
}

// Test updating the SaveImageInfoType
void SaveImageInfoTests::testSaveImageInfoType() {
    SaveImageInfo saveImageInfo;
    saveImageInfo.saveImageInfoType = SaveImageInfoType::SecondImage;

    QCOMPARE(saveImageInfo.saveImageInfoType, SaveImageInfoType::SecondImage);
}

// Test assigning a QPixmap to the image property
void SaveImageInfoTests::testPixmapAssignment() {
    QPixmap pixmap(50, 50);
    QPainter painter(&pixmap);
    painter.fillRect(0, 0, 50, 50, Qt::blue);
    painter.end();

    SaveImageInfo saveImageInfo;
    saveImageInfo.image = pixmap;

    QVERIFY(!saveImageInfo.image.isNull());
    QCOMPARE(saveImageInfo.image.size(), QSize(50, 50));
}
