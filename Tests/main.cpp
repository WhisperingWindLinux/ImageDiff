#include "tst_recentfilesmanager.h"
#include "tst_testrecentfilesinteractor.h"
#include "tst_imagevalidationrules.h"


int main(int argc, char *argv[]) {
    int status = 0;

    QGuiApplication app(argc, argv);

    {
        TestRecentFilesManager test;
        status |= QTest::qExec(&test, argc, argv);
    }

    {
        TestRecentFilesInteractor test;
        status |= QTest::qExec(&test, argc, argv);
    }

    {
        TestImageValidationRules test;
        status |= QTest::qExec(&test, argc, argv);
    }

    return status;
}
