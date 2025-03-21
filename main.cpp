#include <presentation/mainwindow.h>

#include <QApplication>
#include <QtTest>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Image Diff");
    w.show();

    if (argc == 3) {
        QString firstFilePath = argv[1];
        QString SecondFilePath = argv[2];
        w.openImagesFromCommandLine(firstFilePath, SecondFilePath);
    }

    return a.exec();
}
