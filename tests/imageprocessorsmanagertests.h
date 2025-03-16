#ifndef IMAGEPROCESSORSMANAGERTESTS_H
#define IMAGEPROCESSORSMANAGERTESTS_H

#include <QtTest>

class ImageProcessorsManager;

class ImageProcessorsManagerTests : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testFindProcessor();
    void testAddProcessor();
    void testRemoveProcessor();
    void testAllProcessorsInfo();
    void testAllProcessorsInfoTheSameHotkeys();

private:
    ImageProcessorsManager *manager;
};

#endif // IMAGEPROCESSORSMANAGERTESTS_H
