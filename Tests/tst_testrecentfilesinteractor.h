#ifndef TST_TESTRECENTFILESINTERACTOR_H
#define TST_TESTRECENTFILESINTERACTOR_H

#include <QTest>

class TestRecentFilesInteractor : public QObject {
    Q_OBJECT

private slots:
    void testAddRecentFilesRecord();
    void testGetRecentFilesMenuRecords();
    void testClear();
};

#endif // TST_TESTRECENTFILESINTERACTOR_H
