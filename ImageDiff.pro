QT       += core gui testlib multimedia multimediawidgets concurrent printsupport pdf


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comporators/differenceinpixelvaluesasimage.cpp \
    comporators/imageproximitytoorigincomparator.cpp \
    comporators/sharpnesscomparator.cpp \
    gui/delegates/mainwindowbuildmenudelegate.cpp \
    gui/formattors/recentfilespathformater.cpp \
    gui/getimagesfromvideosdialog.cpp \
    gui/videoplayerwidget.cpp \
    imageprocessorsmanager/property.cpp \
    gui/aboutdialog.cpp \
    gui/colorinfopanel.cpp \
    comporators/colorssaturationcomporator.cpp \
    imageprocessorsmanager/comparableimage.cpp \
    imageprocessorsmanager/comparisonresultvariant.cpp \
    comporators/contrastcomporator.cpp \
    gui/imageviewer.cpp \
    gui/propertyeditordialog.cpp \
    imageprocessorsmanager/imageprocessorsmanager.cpp \
    interactors/comparisoninteractor.cpp \
    interactors/getimagesfromvideosinteractor.cpp \
    interactors/recentfilesmanager.cpp \
    interactors/rgbtrackinginteractor.cpp \
    interactors/runallcomparatorsinteractor.cpp \
    interactors/setpropertiesinteractor.cpp \
    interactors/formatters/htmlreportformatter.cpp \
    main.cpp \
    mainwindow.cpp \
    comporators/pixelsabsolutevaluecomparator.cpp \
    comporators/pixelsbrightnesscomparator.cpp \
    tests/comparableimagetests.cpp \
    tests/comparisoninteractortests.cpp \
    tests/comparisonresultvarianttests.cpp \
    tests/imageprocessorsmanagertests.cpp \
    tests/propertytests.cpp \
    tests/recentfilemanagertest.cpp \
    tests/recentfilespathformatertest.cpp \
    tests/saveimageinfotests.cpp \
    tests/setpropertiesinteractortests.cpp \
    filters/grayscalefilter.cpp \
    filters/rgbfilter.cpp

HEADERS += \
    comporators/differenceinpixelvaluesasimage.h \
    comporators/imageproximitytoorigincomparator.h \
    comporators/sharpnesscomparator.h \
    gui/RgbValue.h \
    gui/delegates/mainwindowbuildmenudelegate.h \
    gui/formattors/recentfilespathformater.h \
    gui/getimagesfromvideosdialog.h \
    gui/imageviewstate.h \
    gui/videoplayerwidget.h \
    imageprocessorsmanager/imageprocessorsinfo.h \
    imageprocessorsmanager/imageprocessorsmanager.h \
    imageprocessorsmanager/property.h \
    gui/aboutdialog.h \
    gui/colorinfopanel.h \
    comporators/colorssaturationcomporator.h \
    imageprocessorsmanager/comparableimage.h \
    imageprocessorsmanager/comparisonresultvariant.h \
    comporators/contrastcomporator.h \
    gui/imageviewer.h \
    gui/propertyeditordialog.h \
    interactors/comparisoninteractor.h \
    interactors/getimagesfromvideosinteractor.h \
    interactors/recentfilesmanager.h \
    interactors/rgbtrackinginteractor.h \
    interactors/runallcomparatorsinteractor.h \
    interactors/setpropertiesinteractor.h \
    interfaces/comporator.h \
    interfaces/filter.h \
    interfaces/imageprocessor.h \
    interfaces/iprogressdialog.h \
    interfaces/mainwindowcallbacks.h \
    interactors/formatters/htmlreportformatter.h \
    mainwindow.h \
    comporators/pixelsabsolutevaluecomparator.h \
    comporators/pixelsbrightnesscomparator.h \
    interactors/savefileinfo.h \
    tests/comparableimagetests.h \
    tests/comparisoninteractortests.h \
    tests/comparisonresultvarianttests.h \
    tests/imageprocessorsmanagertests.h \
    tests/mocks/MockImageProcessor.h \
    tests/mocks/mockmainwindowcallbacks.h \
    tests/propertytests.h \
    tests/recentfilemanagertest.h \
    tests/recentfilespathformatertest.h \
    tests/saveimageinfotests.h \
    tests/setpropertiesinteractortests.h \
    tests/testutils.h \
    filters/grayscalefilter.h \
    filters/rgbfilter.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
