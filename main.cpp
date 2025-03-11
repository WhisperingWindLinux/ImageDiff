#include "mainwindow.h"

#include <QApplication>
#include <qfiledialog.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Image Diff");
    w.show();
    return a.exec();
}
