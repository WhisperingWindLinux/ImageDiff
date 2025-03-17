#include "mainwindowbuildmenudelegate.h"
#include <domain/interfaces/imageprocessor.h>
#include <business/imageanalysis/imageprocessorsmanager.h>
#include <presentation/mainwindow.h>
#include <qmenu.h>


MainWindowBuildMenuDelegate::MainWindowBuildMenuDelegate(MainWindow *mainWindow)
    : mainWindow(mainWindow)
{

}

// The application can run comparators and filters - custom image processors classes
// that can be easily added to the application. For more information see the constructor'
// imageprocessormanager/imageprocessormanager.cpp

void MainWindowBuildMenuDelegate::buildFiltersAndComparatorsMenus(QMenu *comparatorsMenu,
                                                                  QMenu *filtersMenu,
                                                                  QMenu *helpMenu
                                                                  )
{
    ImageProcessorsManager *manager = ImageProcessorsManager::instance();
    auto comporatorsInfo = manager->getAllProcessorsInfo();

    // an image comporators
    for (auto it = comporatorsInfo.begin(); it != comporatorsInfo.end(); ++it) {
        QString name = (*it).name;
        QString hotkey = (*it).hotkey;
        ImageProcessorType type = (*it).type;
        QAction *newRunProcessorAction;

        if (type == ImageProcessorType::Comparator) {
            newRunProcessorAction = comparatorsMenu->addAction(name);
        } else if (type == ImageProcessorType::Filter) {
            newRunProcessorAction = filtersMenu->addAction(name);
        } else {
            continue;
        }
        newRunProcessorAction->setData(name);
        newRunProcessorAction->setShortcut(QKeySequence(hotkey));
        mainWindow->connect(newRunProcessorAction,
                            &QAction::triggered,
                            mainWindow,
                            &MainWindow::callImageProcessor
                            );

        QAction *newProcessorHelpAction = helpMenu->addAction(name);
        newProcessorHelpAction->setData(name);
        mainWindow->connect(newProcessorHelpAction,
                            &QAction::triggered,
                            mainWindow,
                            &MainWindow::callImageProcessorHelp
                            );
    }
}





