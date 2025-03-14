QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comporators/dummycomparator.cpp \
    comporators/imageproximitytoorigincomparator.cpp \
    comporators/sharpnesscomparator.cpp \
    gui/formattors/recentfilespathformater.cpp \
    imageprocessorsmanager/property.cpp \
    gui/aboutdialog.cpp \
    gui/colorinfopanel.cpp \
    comporators/colorssaturationcomporator.cpp \
    imageprocessorsmanager/comparableimage.cpp \
    imageprocessorsmanager/comparisonresultvariant.cpp \
    comporators/contrastcomporator.cpp \
    comporators/imagecomparator.cpp \
    gui/imageviewer.cpp \
    gui/propertyeditordialog.cpp \
    imageprocessorsmanager/imageprocessorsmanager.cpp \
    interactors/comparisoninteractor.cpp \
    interactors/plugininteractor.cpp \
    interactors/recentfilesmanager.cpp \
    interactors/setpropertiesinteractor.cpp \
    main.cpp \
    mainwindow.cpp \
    comporators/pixelsabsolutevaluecomparator.cpp \
    comporators/pixelsbrightnesscomparator.cpp \
    plugins/pluginmanager.cpp \
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
    comporators/dummycomparator.h \
    comporators/imageproximitytoorigincomparator.h \
    comporators/sharpnesscomparator.h \
    gui/RgbValue.h \
    gui/formattors/recentfilespathformater.h \
    gui/imageviewstate.h \
    imageprocessorsmanager/imageprocessorsinfo.h \
    imageprocessorsmanager/imageprocessorsmanager.h \
    imageprocessorsmanager/property.h \
    gui/aboutdialog.h \
    gui/colorinfopanel.h \
    comporators/colorssaturationcomporator.h \
    imageprocessorsmanager/comparableimage.h \
    imageprocessorsmanager/comparisonresultvariant.h \
    comporators/contrastcomporator.h \
    comporators/imagecomparator.h \
    gui/imageviewer.h \
    gui/propertyeditordialog.h \
    interactors/comparisoninteractor.h \
    interactors/plugininteractor.h \
    interactors/recentfilesmanager.h \
    interactors/setpropertiesinteractor.h \
    interfaces/comporator.h \
    interfaces/filter.h \
    interfaces/imageprocessor.h \
    interfaces/iplugininterface.h \
    interfaces/mainwindowcallbacks.h \
    mainwindow.h \
    comporators/pixelsabsolutevaluecomparator.h \
    comporators/pixelsbrightnesscomparator.h \
    interactors/savefileinfo.h \
    plugins/pluginmanager.h \
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
