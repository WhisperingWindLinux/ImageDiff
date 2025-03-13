#include "mainwindow.h"

#include <QApplication>
#include <QtTest>
#include <qfiledialog.h>

#include <tests/PropertyTests.h>
#include <tests/comparableimagetests.h>
#include <tests/comparisoninteractortests.h>
#include <tests/comparisonresultvarianttests.h>
#include <tests/imageprocessorsmanagertests.h>
#include <tests/recentfilemanagertest.h>
#include <tests/recentfilespathformatertest.h>
#include <tests/saveimageinfotests.h>
#include <tests/setpropertiesinteractortests.h>

int runTests(int &argc, char **&argv) {
    QVector<QObject*> tests = {
        new PropertyTests(),
        new ImageProcessorsManagerTests(),
        new ComparisonResultVariantTests(),
        new ComparableImageTests(),
        new SetPropertiesInteractorTests(),
        new SaveImageInfoTests(),
        new ComparisonInteractorTests(),
        new RecentFilesPathFormatorTest(),
        new RecentFileManagerTest()
    };

    int retCode = 0;

    for (QObject* test : tests) {
        if (QTest::qExec(test, argc, argv) != 0) {
            qCritical() << test->metaObject()->className() << "Failed!";
            retCode = -1;
        }
        delete test;
    }

    return retCode;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QStringList arguments = QCoreApplication::arguments();
    if (arguments.contains("--run-tests") || !true) {
        return runTests(argc, argv);
    }

    MainWindow w;
    w.setWindowTitle("Image Diff");
    w.show();
    return a.exec();
}
