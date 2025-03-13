#ifndef RECENTFILESPATHFORMATORTESTS_H
#define RECENTFILESPATHFORMATORTESTS_H

#include <QObject>
#include <QtTest>

class RecentFilesPathFormatorTest : public QObject {
    Q_OBJECT

private slots:
    void testPairToString_ValidInput();
    void testPairToString_EmptyInput();
    void testStringToPair_ValidInput();
    void testStringToPair_InvalidInput();
    void testStringToPair_EmptyString();
};

#endif // RECENTFILESPATHFORMATORTESTS_H
