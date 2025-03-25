#ifndef TST_IMAGEVALIDATIONRULES_H
#define TST_IMAGEVALIDATIONRULES_H

#include <QTest>

class TestImageValidationRules : public QObject {
    Q_OBJECT

private slots:
    void testBothImagesAreNull();
    void testDifferentSizes();
    void testValidImages();
};


#endif // TST_IMAGEVALIDATIONRULES_H
