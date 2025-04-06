#ifndef TST_RECENTFILESMANAGER_H
#define TST_RECENTFILESMANAGER_H

#include <QTest>

class TestRecentFilesManager : public QObject {
    Q_OBJECT

private slots:
    void testAddPair();
    void testRecordsTrimmer();
    void testClear();
};

#endif // TST_RECENTFILESMANAGER_H
