#ifndef MAINWINDOWBUILDMENUDELEGATE_H
#define MAINWINDOWBUILDMENUDELEGATE_H

class MainWindow;
class QMenu;

class MainWindowBuildMenuDelegate
{
public:
    MainWindowBuildMenuDelegate(MainWindow *mainWindow);
    void buildFiltersAndComparatorsMenus(QMenu *comparatorsMenu, QMenu *filtersMenu, QMenu *helpMenu);
    void buildHelpFiltersAndComparatorsMenus(QMenu *helpMenu);

private:
    MainWindow *mainWindow;
};

#endif // MAINWINDOWBUILDMENUDELEGATE_H
