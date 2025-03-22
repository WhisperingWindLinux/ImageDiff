#ifndef IMAGEPROCESSORSMENUCONTROLLER_H
#define IMAGEPROCESSORSMENUCONTROLLER_H

#include <domain/valueobjects/imageprocessorsinfo.h>

class MainWindow;
class QMenu;

class ImageProcessorsMenuController
{
public:
    ImageProcessorsMenuController(MainWindow *mainWindow);

    void buildFiltersAndComparatorsMenus(QMenu *comparatorsMenu,
                                         QMenu *filtersMenu,
                                         const QList<ImageProcessorInfo> &imageProcessorsInfo
                                         );
private:
    MainWindow *mainWindow;
};

#endif // IMAGEPROCESSORSMENUCONTROLLER_H
