#ifndef RECENTFILEMANAGERTEST_H
#define RECENTFILEMANAGERTEST_H

#include <QObject>
#include <QtTest>

class RecentFileManagerTest : public QObject {
    Q_OBJECT

public:
    RecentFileManagerTest() = default;

private:
    const QString TEST_ORGANIZATION = "com.whisperingwind";
    const QString TEST_APPLICATION = "ImageDiff";
    const int MAX_RECENT_PAIRS = 3;

private slots:
    void init(); // Runs before each test
    void cleanup(); // Runs after each test

    void testAddPair_AddsNewPair();
    void testAddPair_RemovesDuplicates();
    void testAddPair_TrimsExcessPairs();

    void testClear_RemovesAllPairs();

    void testGetAllPairs_ReturnsCorrectPairs();

    void testSaveToSettings_PersistsData();
    void testLoadFromSettings_LoadsData();
};

#endif // RECENTFILEMANAGERTEST_H
