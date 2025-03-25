#include "tst_testrecentfilesinteractor.h"

#include <business/recentfilesinteractor.h>
#include <tests/mocks/mockrecentfilesmanager.h>


void TestRecentFilesInteractor::testAddRecentFilesRecord() {
    IRecentFilesManager *recentFileManager = new MockRecentFilesManager();
    RecentFilesInteractor interactor(recentFileManager);

    interactor.addRecentFilesRecord("file1.png", "file2.png");
    interactor.addRecentFilesRecord("file3.png", "file4.png");
    interactor.addRecentFilesRecord("file5.png", "file6.png");
    QStringList records = interactor.getRecentFilesMenuRecords();

    QCOMPARE(records.size(), 3);
    QVERIFY(records.contains("file1.png -> file2.png"));
    QVERIFY(records.contains("file3.png -> file4.png"));
    QVERIFY(records.contains("file5.png -> file6.png"));
}

void TestRecentFilesInteractor::testGetRecentFilesMenuRecords() {
    IRecentFilesManager *recentFileManager = new MockRecentFilesManager();
    RecentFilesInteractor interactor(recentFileManager);

    interactor.addRecentFilesRecord("file3.png", "file4.png");
    interactor.addRecentFilesRecord("file1.png", "file2.png");

    QStringList records = interactor.getRecentFilesMenuRecords();

    QCOMPARE(records.size(), 2);
    QCOMPARE(records[0], "file1.png -> file2.png");
    QCOMPARE(records[1], "file3.png -> file4.png");
}

void TestRecentFilesInteractor::testClear() {
    IRecentFilesManager *recentFileManager = new MockRecentFilesManager();
    RecentFilesInteractor interactor(recentFileManager);

    interactor.addRecentFilesRecord("file1.png", "file2.png");
    interactor.clear();

    QStringList records = interactor.getRecentFilesMenuRecords();
    QCOMPARE(records.size(), 0);
}

