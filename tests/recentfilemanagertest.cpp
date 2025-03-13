#include "recentfilemanagertest.h"

#include <interactors/recentfilesmanager.h>


void RecentFileManagerTest::init() {
    // Clear settings before each test
    QSettings settings(TEST_ORGANIZATION, TEST_APPLICATION);
    settings.clear();
}

void RecentFileManagerTest::cleanup() {
    // Clear settings after each test
    QSettings settings(TEST_ORGANIZATION, TEST_APPLICATION);
    settings.clear();
}

void RecentFileManagerTest::testAddPair_AddsNewPair() {
    RecentFilesManager manager(TEST_ORGANIZATION, TEST_APPLICATION, MAX_RECENT_PAIRS);

    manager.addPair("file1.txt", "file2.txt");

    QList<QPair<QString, QString>> pairs = manager.getAllPairs();
    QCOMPARE(pairs.size(), 1);
    QCOMPARE(pairs.first(), qMakePair(QString("file1.txt"), QString("file2.txt")));
}

void RecentFileManagerTest::testAddPair_RemovesDuplicates() {
    RecentFilesManager manager(TEST_ORGANIZATION, TEST_APPLICATION, MAX_RECENT_PAIRS);

    manager.addPair("file1.txt", "file2.txt");
    manager.addPair("file1.txt", "file2.txt"); // Duplicate pair

    QList<QPair<QString, QString>> pairs = manager.getAllPairs();
    QCOMPARE(pairs.size(), 1); // Ensure no duplicates
}

void RecentFileManagerTest::testAddPair_TrimsExcessPairs() {
    RecentFilesManager manager(TEST_ORGANIZATION, TEST_APPLICATION, MAX_RECENT_PAIRS);

    // Add more pairs than the maximum allowed
    manager.addPair("file1.txt", "file2.txt");
    manager.addPair("file3.txt", "file4.txt");
    manager.addPair("file5.txt", "file6.txt");
    manager.addPair("file7.txt", "file8.txt");

    QList<QPair<QString, QString>> pairs = manager.getAllPairs();
    QCOMPARE(pairs.size(), MAX_RECENT_PAIRS); // Ensure list size is trimmed
    QCOMPARE(pairs.first(), qMakePair(QString("file7.txt"), QString("file8.txt"))); // Most recent pair
}

void RecentFileManagerTest::testClear_RemovesAllPairs() {
    RecentFilesManager manager(TEST_ORGANIZATION, TEST_APPLICATION, MAX_RECENT_PAIRS);

    manager.addPair("file1.txt", "file2.txt");
    manager.clear();

    QList<QPair<QString, QString>> pairs = manager.getAllPairs();
    QCOMPARE(pairs.size(), 0); // Ensure list is empty
}

void RecentFileManagerTest::testGetAllPairs_ReturnsCorrectPairs() {
    RecentFilesManager manager(TEST_ORGANIZATION, TEST_APPLICATION, MAX_RECENT_PAIRS);

    manager.addPair("file1.txt", "file2.txt");
    manager.addPair("file3.txt", "file4.txt");

    QList<QPair<QString, QString>> pairs = manager.getAllPairs();
    QCOMPARE(pairs.size(), 2);
    QCOMPARE(pairs[0], qMakePair(QString("file3.txt"), QString("file4.txt"))); // Most recent pair first
    QCOMPARE(pairs[1], qMakePair(QString("file1.txt"), QString("file2.txt"))); // Older pair second
}

void RecentFileManagerTest::testSaveToSettings_PersistsData() {
    {
        RecentFilesManager manager(TEST_ORGANIZATION, TEST_APPLICATION, MAX_RECENT_PAIRS);
        manager.addPair("file1.txt", "file2.txt");
        manager.addPair("file3.txt", "file4.txt");
        // Manager goes out of scope and should save to settings
    }

    QSettings settings(TEST_ORGANIZATION, TEST_APPLICATION);
    QVariantList savedPairs = settings.value(RecentFilesManager::SETTINGS_KEY).toList();

    QCOMPARE(savedPairs.size(), 2*2); // Ensure data is saved correctly
}

void RecentFileManagerTest::testLoadFromSettings_LoadsData() {
    RecentFilesManager manager1(TEST_ORGANIZATION, TEST_APPLICATION, MAX_RECENT_PAIRS);
    manager1.addPair("file1.txt", "file2.txt");
    manager1.addPair("file3.txt", "file4.txt");

    // Load data into RecentFilesManager
    RecentFilesManager manager2(TEST_ORGANIZATION, TEST_APPLICATION, MAX_RECENT_PAIRS);
    QList<QPair<QString, QString>> pairs = manager2.getAllPairs();
    QCOMPARE(pairs.size(), 2); // Ensure data is loaded correctly
    QCOMPARE(pairs[1], qMakePair(QString("file1.txt"), QString("file2.txt")));
    QCOMPARE(pairs[0], qMakePair(QString("file3.txt"), QString("file4.txt")));
}
