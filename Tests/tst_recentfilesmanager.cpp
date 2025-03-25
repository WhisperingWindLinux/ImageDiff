#include "tst_recentfilesmanager.h"

#include <business/recentfilesinteractor.h>
#include <tests/mocks/mockrecentfilesmanager.h>


void TestRecentFilesManager::testAddPair() {
    RecentFilesManager manager("testAddPair", "app");

    QStringPair pair1 = {"file1.png", "file2.png"};
    QStringPair pair2 = {"file3.png", "file4.png"};
    QStringPair pair3 = {"file5.png", "file6.png"};

    manager.addPair(pair1.first, pair1.second);
    manager.addPair(pair2.first, pair2.second);
    manager.addPair(pair3.first, pair3.second);

    QList<QStringPair> records = manager.getAllPairs();

    QCOMPARE(records.size(), 3);
    QCOMPARE(records[0], pair3);
    QCOMPARE(records[1], pair2);
    QCOMPARE(records[2], pair1);

    manager.clear();
}

void TestRecentFilesManager::testRecordsTrimmer() {
    RecentFilesManager manager("testRecordsTrimmer", "app", 4);

    QStringPair pair1 = {"file1.png", "file2.png"};
    QStringPair pair2 = {"file3.png", "file4.png"};
    QStringPair pair3 = {"file5.png", "file6.png"};
    QStringPair pair4 = {"file7.png", "file8.png"};
    QStringPair pair5 = {"file9.png", "file10.png"};

    manager.addPair(pair1.first, pair1.second);
    manager.addPair(pair2.first, pair2.second);
    manager.addPair(pair3.first, pair3.second);
    manager.addPair(pair4.first, pair4.second);
    manager.addPair(pair5.first, pair5.second);

    QList<QStringPair> records = manager.getAllPairs();

    QCOMPARE(records.size(), 4);
    QCOMPARE(records[0], pair5);
    QCOMPARE(records[1], pair4);
    QCOMPARE(records[2], pair3);
    QCOMPARE(records[3], pair2);

    QVERIFY(!records.contains(pair1));

    manager.clear();
}

void TestRecentFilesManager::testClear() {
    RecentFilesManager manager("testClear", "app");

    QStringPair pair1 = {"file1.png", "file2.png"};
    QStringPair pair2 = {"file3.png", "file4.png"};
    QStringPair pair3 = {"file5.png", "file6.png"};

    manager.addPair(pair1.first, pair1.second);
    manager.addPair(pair2.first, pair2.second);
    manager.addPair(pair3.first, pair3.second);

    QList<QStringPair> records = manager.getAllPairs();
    QCOMPARE(records.size(), 3);
    manager.clear();
    records = manager.getAllPairs();
    QCOMPARE(records.size(), 0);
}

