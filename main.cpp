#include <presentation/mainwindow.h>

#include <QApplication>


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

#ifndef TARGET_OS_MAC
    #error The application was developed and tested for functionality only on macOS.
    #error Functionality or even the possibility of building it for other operating systems is not guaranteed.
    #error If you're feeling lucky, you can try to build and run the application on your chosen operating system.
#endif

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
