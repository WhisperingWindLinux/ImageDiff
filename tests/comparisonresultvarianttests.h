#ifndef COMPARISONRESULTVARIANTTESTS_H
#define COMPARISONRESULTVARIANTTESTS_H

#include <QObject>

class ComparisonResultVariantTests : public QObject {
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testImageConstructor();
    void testStringConstructor();
    void testImageResult();
    void testStringResult();
    void testType();
};

#endif // COMPARISONRESULTVARIANTTESTS_H
