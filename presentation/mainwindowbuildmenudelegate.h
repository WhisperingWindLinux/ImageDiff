#ifndef MAINWINDOWBUILDMENUDELEGATE_H
#define MAINWINDOWBUILDMENUDELEGATE_H

#include <domain/valueobjects/imageprocessorsinfo.h>


class MainWindow;
class QMenu;

class MainWindowBuildMenuDelegate
{
public:
    MainWindowBuildMenuDelegate(MainWindow *mainWindow);

    void buildFiltersAndComparatorsMenus(QMenu *comparatorsMenu,
                                         QMenu *filtersMenu,
                                         QList<ImageProcessorInfo> imageProcessorsInfo
                                         );

    void buildHelpFiltersAndComparatorsMenus(QMenu *helpMenu);

private:
    MainWindow *mainWindow;
};

#endif // MAINWINDOWBUILDMENUDELEGATE_H
