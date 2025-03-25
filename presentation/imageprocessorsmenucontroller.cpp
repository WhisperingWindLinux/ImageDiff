#include "imageprocessorsmenucontroller.h"
#include <domain/interfaces/business/imageprocessor.h>
#include <business/imageanalysis/imageprocessorsmanager.h>
#include <presentation/mainwindow.h>
#include <qmenu.h>


ImageProcessorsMenuController::ImageProcessorsMenuController(MainWindow *mainWindow)
    : mainWindow(mainWindow)
{

}

// The application can run comparators and filters - custom image processors classes
// that can be easily added to the application. For more information see the constructor'
// imageprocessormanager/imageprocessormanager.cpp

void ImageProcessorsMenuController::buildFiltersAndComparatorsMenus(QMenu *comparatorsMenu,
                                                                    QMenu *filtersMenu,
                                                                    const QList<ImageProcessorInfo> &imageProcessorsInfo
                                                                    )
{
    comparatorsMenu->clear();
    filtersMenu->clear();

    for (auto it = imageProcessorsInfo.begin(); it != imageProcessorsInfo.end(); ++it) {
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
    }
}
